#!/usr/bin/python3

import argparse
import select
from src.server_link import ZappyNetwork
from src.command import ZappyCommand
from src.Ai_agent import ZappyAgent
from src.logger import ZappyLogger

def wait_for_welcome(net):
    """Attend que le serveur envoie "WELCOME" pour confirmer la connexion.
    Params:
        -net (ZappyNetwork): L'instance de connexion réseau.
    Retourne True si WELCOME reçu, False en cas de timeout ou d'erreur."""

    print("[Handshake] En attente du WELCOME...")
    welcome_received = False
    iterations = 0
    max_iterations = 50
    
    while iterations < max_iterations and not welcome_received:
        messages = net.receive_messages()
        for msg in messages:
            if msg == "WELCOME":
                welcome_received = True
                print("[Handshake] WELCOME reçu")
                return True
        if not welcome_received:
            select.select([net.sock], [], [])
            iterations += 1
    print("[Erreur] WELCOME pas reçu du serveur (timeout)")
    return False

def wait_for_infos(net):
    """Attend que le serveur envoie le CLIENT-NUM et les dimensions de la carte.
    Params:
        -net (ZappyNetwork): L'instance de connexion réseau.
    Retourne un tuple (success, client_num, dimensions) où success est True si les infos sont reçues, False sinon."""

    print("[Handshake] En attente de CLIENT-NUM et dimensions...")
    client_num = None
    dimensions = None
    iterations = 0
    max_iterations = 50 
    
    while iterations < max_iterations and (client_num is None or dimensions is None):
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
                        print("[Erreur] 0 slots restants.")
                        return False, 0, None
                elif dimensions is None and ' ' in msg:
                    parts = msg.split()
                    if len(parts) == 2 and all(p.isdigit() for p in parts):
                        dimensions = (int(parts[0]), int(parts[1]))
                        print(f"[Handshake] Dimensions: {dimensions[0]}x{dimensions[1]}")
            except (ValueError, IndexError):
                pass
        if client_num is None or dimensions is None:
            select.select([net.sock], [], [])
            iterations += 1
    if client_num is None or dimensions is None:
        print(f"[Erreur] Handshake incomplet (CLIENT-NUM: {client_num}, Dimensions: {dimensions})")
        return False, None, None
    return True, client_num, dimensions

def handshake(net, team_name):
    """Effectue le handshake initial avec le serveur: attend WELCOME, envoie le nom de team, attend CLIENT-NUM et dimensions.
    Params:
        -net (ZappyNetwork): L'instance de connexion réseau.
        -team_name (str): Le nom de l'équipe.
    Retourne True si le handshake est réussi, False sinon."""

    if not wait_for_welcome(net):
        return False, None, None
    print(f"[Handshake] Envoi du nom de team: {team_name}")
    if not net.send_command(team_name):
        print("[Erreur] Impossible d'envoyer le team_name")
        return False, None, None
    success, client_num, dimensions = wait_for_infos(net)
    if not success:
        return False, None, None
    print("[Handshake] Succès! Démarrage de l'agent...")
    return True, client_num, dimensions

def main(port, machine, team_name):
    """Point d'entrée principal du client Zappy. Gère la connexion, le handshake, et lance l'agent.
    Params:
        -port (int): Le port du serveur.
        -machine (str): L'adresse du serveur (nom de machine).
        -team_name (str): Le nom de l'équipe.
    Retourne 0 en cas de succès, 84 en cas d'erreur."""

    ZappyLogger.setup(ZappyLogger,team_name, debug_mode=True)
    logger = ZappyLogger.get_logger("Main")
    logger.info(f"Démarrage du client Zappy (Team: {team_name}, Port: {port})")
    net = ZappyNetwork(machine, port)
    try:
        net.connect()
    except Exception as e:
        logger.error(f"Impossible de se connecter: {e}")
        return 84
    if not net.connected:
        return 84
    is_connected, client_num, dimensions = handshake(net, team_name)
    if not is_connected:
        net.close()
        return 84
    try:
        cmd_interface = ZappyCommand(net)
        agent = ZappyAgent(cmd_interface, team_name, dimensions, client_num)
        agent.run()
        logger.info("[Agent] Agent terminé correctement")
    except KeyboardInterrupt:
        logger.info("\n[Agent] Arrêt par l'utilisateur")
    except Exception as e:
        logger.error(f"[Erreur] Exception dans l'agent: {e}")
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
