#!/usr/bin/python3

import time
import argparse
from src.server_link import ZappyNetwork
from src.command import ZappyCommand
from src.Ai_agent import ZappyAgent

def handshake(net, team_name):
    print("[Handshake] En attente du WELCOME...")
    welcome_received = False
    timeout = 0
    max_timeout = 5000
    
    while timeout < max_timeout and not welcome_received:
        messages = net.receive_messages()
        for msg in messages:
            if msg == "WELCOME":
                welcome_received = True
                print("[Handshake] WELCOME reçu")
                break
        if not welcome_received:
            time.sleep(0.1)
            timeout += 100
    
    if not welcome_received:
        print("[Erreur] WELCOME pas reçu du serveur (timeout)")
        return False
    print(f"[Handshake] Envoi du nom de team: {team_name}")
    if not net.send_command(team_name):
        print("[Erreur] Impossible d'envoyer le team")
        return False
    print("[Handshake] En attente de CLIENT-NUM et dimensions...")
    client_num = None
    dimensions = None
    timeout = 0
    max_timeout = 5000 
    
    while timeout < max_timeout and (not client_num or not dimensions):
        messages = net.receive_messages()
        for msg in messages:
            try:
                if not client_num and msg.isdigit():
                    client_num = int(msg)
                    print(f"[Handshake] CLIENT-NUM: {client_num}")
                elif not dimensions and ' ' in msg:
                    parts = msg.split()
                    if len(parts) == 2 and all(p.isdigit() for p in parts):
                        dimensions = (int(parts[0]), int(parts[1]))
                        print(f"[Handshake] Dimensions: {dimensions[0]}x{dimensions[1]}")
            except (ValueError, IndexError):
                pass
        
        if not client_num or not dimensions:
            time.sleep(0.1)
            timeout += 100
    
    if not client_num or not dimensions:
        print(f"[Erreur] Handshake incomplet (CLIENT-NUM: {client_num}, Dimensions: {dimensions})")
        return False
    
    print("[Handshake] Succès! Démarrage de l'agent...")
    return True

def main(port, machine, team_name):
    net = ZappyNetwork(machine, port)
    try:
        net.connect()
    except Exception as e:
        print(f"[Erreur] Impossible de se connecter: {e}")
        return 84
    if not net.connected:
        return 84
    if not handshake(net, team_name):
        net.close()
        return 84
    try:
        cmd_interface = ZappyCommand(net)
        agent = ZappyAgent(cmd_interface, team_name)
        agent.run()
        print("[Agent] Agent terminé correctement")
    except KeyboardInterrupt:
        print("\n[Agent] Arrêt par l'utilisateur")
    except Exception as e:
        print(f"[Erreur] Exception dans l'agent: {e}")
        import traceback
        traceback.print_exc()
        return 84
    finally:
        net.close()
    
    return 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Zappy AI Client', add_help=False)
    parser.add_argument('-p', '--port', type=int, default=4243, help='Port du serveur')
    parser.add_argument('-h', '--host', type=str, default='localhost', help='Adresse du serveur (nom de machine)')
    parser.add_argument('-n', '--name', type=str, default='Team1', help='Nom du team')
    parser.add_argument('--help', action='help', help='Affiche cette aide')
    args = parser.parse_args()
    exit(main(args.port, args.host, args.name))
