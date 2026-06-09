#!/usr/bin/python3

import time
import argparse
from src.server_link import ZappyNetwork
from src.command import ZappyCommand
from src.Ai_agent import ZappyAgent

def wait_for_welcome(net):
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
                return True
        if not welcome_received:
            time.sleep(0.1)
            timeout += 100
    print("[Erreur] WELCOME pas reçu du serveur (timeout)")
    return False

def wait_for_infos(net):
    print("[Handshake] En attente de CLIENT-NUM et dimensions...")
    client_num = None
    dimensions = None
    timeout = 0
    max_timeout = 5000 
    
    while timeout < max_timeout and (client_num is None or dimensions is None):
        messages = net.receive_messages()
        for msg in messages:
            if msg == "ko":
                print("[Erreur] Le serveur a refusé la team (ko). Nom invalide ou équipe pleine ?")
                return False, None, None
            try:
                if client_num is None and msg.isdigit():
                    client_num = int(msg)
                    print(f"[Handshake] CLIENT-NUM: {client_num}")
                    if client_num == 0:
                        print("[Erreur] L'équipe est pleine (0 slots restants).")
                        return False, 0, None
                elif dimensions is None and ' ' in msg:
                    parts = msg.split()
                    if len(parts) == 2 and all(p.isdigit() for p in parts):
                        dimensions = (int(parts[0]), int(parts[1]))
                        print(f"[Handshake] Dimensions: {dimensions[0]}x{dimensions[1]}")
            except (ValueError, IndexError):
                pass
        if client_num is None or dimensions is None:
            time.sleep(0.1)
            timeout += 100
    if client_num is None or dimensions is None:
        print(f"[Erreur] Handshake incomplet (CLIENT-NUM: {client_num}, Dimensions: {dimensions})")
        return False, None, None
    return True, client_num, dimensions

def handshake(net, team_name):
    if not wait_for_welcome(net):
        return False
    print(f"[Handshake] Envoi du nom de team: {team_name}")
    if not net.send_command(team_name):
        print("[Erreur] Impossible d'envoyer le team")
        return False
    success, client_num, dimensions = wait_for_infos(net)
    if not success:
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
