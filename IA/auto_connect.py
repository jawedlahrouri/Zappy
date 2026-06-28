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


def check_available_slots(machine, port, team_name):
    """
    Se connecte brièvement pour tester le nombre de places disponibles.
    Ferme la connexion immédiatement pour ne pas consommer le slot testé.
    Retourne le nombre de places libres ou 0 en cas de serveur complet/erreur.
    """
    try:
        sock = create_connection(machine, port)
    except Exception:
        return -1  # Serveur injoignable

    try:
        welcome = recv_line(sock, timeout=3.0)
        if welcome != "WELCOME":
            return 0
        if not send_line(sock, team_name):
            return 0
        client_num_str = recv_line(sock, timeout=3.0)
        if not client_num_str or client_num_str == "ko":
            return 0
        try:
            client_num = int(client_num_str)
        except ValueError:
            return 0
        # Lire les dimensions de la carte pour vider le buffer
        recv_line(sock, timeout=1.0)
        # En fermant notre socket de test, on libère le slot qu'on venait d'occuper (+1)
        return client_num + 1
    except Exception:
        return 0
    finally:
        sock.close()


def launch_ai(port, machine, team_name):
    """Lance une instance de zappy_ai en arrière-plan."""
    os.makedirs("logs", exist_ok=True)
    log_path = "logs/clone_crash.log"
    
    script_dir = os.path.dirname(os.path.abspath(__file__))
    zappy_ai_bin = os.path.normpath(os.path.join(script_dir, "..", "zappy_ai"))
    main_py = os.path.join(script_dir, "main.py")
    
    if os.path.exists(zappy_ai_bin) and os.access(zappy_ai_bin, os.X_OK):
        cmd = [zappy_ai_bin, "-p", str(port), "-h", machine, "-n", team_name]
    else:
        cmd = [sys.executable, main_py, "-p", str(port), "-h", machine, "-n", team_name]
        
    with open(log_path, "a") as f:
        subprocess.Popen(
            cmd,
            stdout=f,
            stderr=subprocess.STDOUT,
            start_new_session=True
        )
    print(f"[AutoConnect] Nouvel agent lancé pour l'équipe {team_name}")


def monitor_loop(port, machine, team_name):
    """
    Boucle de surveillance : vérifie régulièrement le nombre de places et lance un agent si nécessaire.
    """
    print(f"[AutoConnect] Surveillance active pour l'équipe '{team_name}' sur {machine}:{port}...")
    server_down_notified = False

    while True:
        slots = check_available_slots(machine, port, team_name)
        if slots == -1:
            if not server_down_notified:
                print("[AutoConnect] Serveur injoignable, en attente de sa disponibilité...")
                server_down_notified = True
            time.sleep(2.0)
            continue
        
        if server_down_notified:
            print("[AutoConnect] Serveur en ligne !")
            server_down_notified = False

        if slots > 0:
            print(f"[AutoConnect] {slots} place(s) disponible(s) -> lancement d'un nouvel agent...")
            launch_ai(port, machine, team_name)
            # Attendre 2 secondes que l'agent se connecte au serveur et occupe sa place
            time.sleep(2.0)
        else:
            time.sleep(1.5)


def main():
    parser = argparse.ArgumentParser(description="Zappy Auto-Connect Monitor", add_help=False)
    parser.add_argument("-p", "--port", type=int, required=True, help="Port du serveur")
    parser.add_argument("-h", "--host", type=str, default="127.0.0.1", dest="machine",
                        help="Adresse du serveur")
    parser.add_argument("-n", "--name", type=str, required=True, help="Nom de l'équipe")
    parser.add_argument("--help", action="help", help="Affiche cette aide")
    args = parser.parse_args()

    try:
        monitor_loop(args.port, args.machine, args.name)
    except KeyboardInterrupt:
        print("\n[AutoConnect] Arrêt demandé.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
