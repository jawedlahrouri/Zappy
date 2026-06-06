#!/usr/bin/python3

import time
from src.server_link import ZappyNetwork


def main(port, machine):
    TEAM_NAME = "skibidi dop dop yes yes\n" 
    net = ZappyNetwork(machine, port)
    net.connect()

    if not net.connected:
        return

    time.sleep(0.1)
    messages = net.receive_messages()
    print(f"[Serveur] -> {messages}")

    if "WELCOME" in messages:
        print(f"[Client]  <- Envoi de: {TEAM_NAME.strip()}")
        net.send_command(TEAM_NAME)
        time.sleep(0.1)
        reponses = net.receive_messages()
        print(f"[Serveur] -> {reponses}")
        print("[Client]  <- Envoi de: Forward")
        net.send_command("paf\n")
        time.sleep(0.2) 
        reponses = net.receive_messages()
        print(f"[Serveur] -> {reponses}")
    net.close()

if __name__ == '__main__':
    main(4242, "localhost")
