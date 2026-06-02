#!/usr/bin/python3

import time
from src.server_link import ZappyNetwork

class ZappyCommand:
    def __init__(self, network):
        self.network = network

    # Méthode de capteurs de l'environnement 
    def look(self):
        print("[Agent] commande: Look")
        self.network.send_command("Look\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        if response == "ko":
            return []
        return self._parse_look(response)

    def _parse_look(self, raw_str):
        parsed_vision = []
        clean_str = raw_str.strip('[]\n')
        tiles = clean_str.split(',')

        for tile in tiles:
            items = tile.strip().split()
            parsed_vision.append(items)
        return parsed_vision
    
    def broadcast_text(self, text):
        print(f"[Agent] commande: Broadcast '{text}'")
        self.network.send_command(f"Broadcast {text}\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    def inventory(self):
        print("[Agent] commande: Inventory")
        self.network.send_command("Inventory\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        if response == "ko":
            return {}
        return self._parse_inventory(response)
    
    def _parse_inventory(self, raw_str):
        inventory = {}
        clean_str = raw_str.strip('[]\n')
        items = clean_str.split(',')
        for item in items:
            key, value = item.strip().split()
            inventory[key] = int(value)
        return inventory
    
    # Méthodes de déplacement
    def forward(self):
        print("[Agent] commande: Forward")
        self.network.send_command("Forward\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    def right(self):
        print("[Agent] commande: Right")
        self.network.send_command("Right\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    def left(self):
        print("[Agent] commande: Left")
        self.network.send_command("Left\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    # Méthodes d'interaction avec l'environnement
    def take_object(self, obj):
        print(f"[Agent] commande: Take {obj}")
        self.network.send_command(f"Take {obj}\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    def set_object(self, obj):
        print(f"[Agent] commande: Set {obj}")
        self.network.send_command(f"Set {obj}\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"


    # Méthode d'incantation pour l'upgrade des ptits persos  là TODO: à implémenter complètement plus tard je préfere d'abord faire les commande "bidon"
    def Incantation(self):
        print("[Agent] commande: Incantation")
        self.network.send_command("Incantation\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"    

    # Méthode d'interaction avec les autres joueurs/avec le jeu

    def Connect_nbr(self):
        print("[Agent] commande: Connect_nbr")
        self.network.send_command("Connect_nbr\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        if response == "ko":
            return -1
        return int(response)
    
    def fork(self):
        print("[Agent] commande: Fork")
        self.network.send_command("Fork\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"

    def eject(self):
        print("[Agent] commande: Eject")
        self.network.send_command("Eject\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "ok"
    
    def death(self):
        print("[Agent] commande: Death")
        self.network.send_command("Death\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        return response == "dead"
