#!/usr/bin/python3

import sys
from src.server_link import ZappyNetwork
from src.command import ZappyCommand
from src.Ai_agent import ZappyAgent

def main(port, machine, team_name):
    team_name_formatted = f"{team_name}\n" 
    net = ZappyNetwork(machine, port)
    net.connect()
    if not net.connected:
        return
    messages = net.receive_messages()
    if "WELCOME" in messages:
        net.send_command(team_name_formatted)
        reponses = net.receive_messages()
        print(f"[Serveur Handshake] -> {reponses}")
        print("Démarrage de l'IA")
        cmd_interface = ZappyCommand(net)
        agent = ZappyAgent(cmd_interface)
        agent.run() 
    net.close()

if __name__ == '__main__':
    main(4242, "localhost", "ZappyBoyz")