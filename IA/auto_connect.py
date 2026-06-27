#!/usr/bin/python3
"""
Script de surveillance des slots serveur.
Se connecte au serveur Zappy, surveille les slots disponibles via Connect_nbr,
et lance automatiquement des instances ./zappy_ai pour combler les places restantes.
"""

import argparse
import subprocess
import select
import socket
import time
import sys
import os


def create_connection(machine, port):
    """Crée une connexion socket vers le serveur."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((machine, port))
    sock.setblocking(False)
    return sock


def recv_line(sock, timeout=5.0):
    """Reçoit une ligne complète du serveur avec timeout."""
    buf = b""
    end_time = time.time() + timeout
    while time.time() < end_time:
        remaining = end_time - time.time()
        if remaining <= 0:
            break
        ready, _, _ = select.select([sock], [], [], min(remaining, 0.1))
        if ready:
            try:
                data = sock.recv(4096)
                if not data:
                    return None
                buf += data
                if b"\n" in buf:
                    lines = buf.split(b"\n")
                    return lines[0].decode().strip()
            except (ConnectionResetError, BrokenPipeError):
                return None
    return buf.decode().strip() if buf else None


def send_line(sock, msg):
    """Envoie un message au serveur."""
    try:
        sock.sendall((msg + "\n").encode())
        return True
    except (ConnectionResetError, BrokenPipeError):
        return False


def do_handshake(sock, team_name):
    """Effectue le handshake avec le serveur. Retourne (success, client_num)."""
    welcome = recv_line(sock, timeout=10.0)
    if welcome != "WELCOME":
        print(f"[AutoConnect] WELCOME non reçu, reçu: {welcome}")
        return False, 0

    if not send_line(sock, team_name):
        return False, 0

    # Recevoir client_num
    client_num_str = recv_line(sock, timeout=5.0)
    if client_num_str is None:
        return False, 0
    try:
        client_num = int(client_num_str)
    except ValueError:
        print(f"[AutoConnect] CLIENT_NUM invalide: {client_num_str}")
        return False, 0

    # Recevoir dimensions (on s'en fiche ici)
    recv_line(sock, timeout=5.0)

    return True, client_num


def launch_ai(port, machine, team_name):
    """Lance une instance de ./zappy_ai en arrière-plan."""
    log_path = "logs/clone_crash.log"
    os.makedirs("logs", exist_ok=True)
    with open(log_path, "a") as f:
        subprocess.Popen(
            ["./zappy_ai", "-p", str(port), "-h", machine, "-n", team_name],
            stdout=f,
            stderr=subprocess.STDOUT,
        )
    print(f"[AutoConnect] Nouveau clone lancé pour {team_name}")


def monitor_loop(sock, port, machine, team_name):
    """
    Boucle principale : envoie régulièrement Connect_nbr
    et lance un ./zappy_ai pour chaque slot libre détecté.
    """
    print("[AutoConnect] Surveillance des slots en cours...")

    while True:
        if not send_line(sock, "Connect_nbr"):
            print("[AutoConnect] Connexion perdue avec le serveur.")
            break

        response = recv_line(sock, timeout=5.0)
        if response is None:
            print("[AutoConnect] Le serveur a fermé la connexion.")
            break

        # Filtrer les broadcasts (messages de la forme "message K,...")
        while response and response.startswith("message "):
            response = recv_line(sock, timeout=2.0)

        if response == "dead":
            print("[AutoConnect] L'agent moniteur est mort. Relance...")
            break

        try:
            available_slots = int(response)
        except (ValueError, TypeError):
            # Réponse inattendue, on réessaie
            time.sleep(0.5)
            continue

        if available_slots > 0:
            print(f"[AutoConnect] {available_slots} slot(s) disponible(s), lancement de clones...")
            for _ in range(available_slots):
                launch_ai(port, machine, team_name)

        # Attendre un peu avant de re-vérifier (Connect_nbr coûte 0 TU mais on
        # ne veut pas spammer non plus)
        time.sleep(1.0)


def main():
    parser = argparse.ArgumentParser(description="Zappy Auto-Connect Monitor")
    parser.add_argument("-p", "--port", type=int, required=True, help="Port du serveur")
    parser.add_argument("-h", "--host", type=str, default="127.0.0.1", dest="machine",
                        help="Adresse du serveur")
    parser.add_argument("-n", "--name", type=str, required=True, help="Nom de l'équipe")
    args = parser.parse_args()

    print(f"[AutoConnect] Connexion au serveur {args.machine}:{args.port} pour l'équipe {args.name}...")

    try:
        sock = create_connection(args.machine, args.port)
    except Exception as e:
        print(f"[AutoConnect] Impossible de se connecter: {e}")
        return 84

    success, client_num = do_handshake(sock, args.name)
    if not success:
        print("[AutoConnect] Handshake échoué.")
        sock.close()
        return 84

    print(f"[AutoConnect] Connecté ! Slots initiaux disponibles: {client_num}")

    # Lancer immédiatement des clones pour les slots initiaux disponibles
    # (Ce sont les slots qui existaient AVANT notre connexion, car notre
    # connexion en a consommé un. Le serveur nous dit combien il en reste.)
    for _ in range(client_num):
        launch_ai(args.port, args.machine, args.name)

    try:
        monitor_loop(sock, args.port, args.machine, args.name)
    except KeyboardInterrupt:
        print("\n[AutoConnect] Arrêt demandé.")
    finally:
        sock.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())
