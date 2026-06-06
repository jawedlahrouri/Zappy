#!/usr/bin/python3

import time
from src.server_link import ZappyNetwork

class ZappyCommand:
    def __init__(self, network):
        self.network = network
        self.broadcast_queue = []
        self.system_queue = []

    def wait_for_incantation_follower(self):
        print("[Agent] FOLLOWER : En attente du rituel...")
        start_time = time.time()
        first_response = None
        
        while time.time() - start_time < 10.0:
            first_response = self._wait_for_response(expect_incantation=True, timeout=1.0)
            if first_response in ["Elevation underway", "ko"]:
                break     
        if first_response == "Elevation underway":
            second_response = self._wait_for_response(expect_incantation=True, timeout=300.0)
            print(f"[Agent] FOLLOWER Incantation (Fin): {second_response}")
            if second_response and second_response.startswith("Current level"):
                return True
        return False

    def _wait_for_response(self, expect_incantation=False, timeout=None):
        start_time = time.time()
        while True:
            if timeout and (time.time() - start_time > timeout):
                return "timeout"
            if self.system_queue:
                msg = self.system_queue.pop(0)
                if not expect_incantation:
                    if msg == "Elevation underway" or msg.startswith("Current level"):
                        print(f"[Réseau] Ignoré (Rituel d'un autre joueur) : {msg}")
                        continue
                return msg
            messages = self.network.receive_messages()
            for msg in messages:
                if msg.startswith("message "):
                    self.broadcast_queue.append(msg)
                elif msg.startswith("eject: "):
                    print(f"[Agent] Aïe ! On s'est fait pousser : {msg}")
                elif msg == "dead":
                    self.system_queue.append("dead")
                else:
                    self.system_queue.append(msg)
            time.sleep(0.05)
    
    def get_unread_broadcasts(self):
        unread = self.broadcast_queue.copy()
        self.broadcast_queue.clear()
        return unread

    def look(self):
        print("[Agent] commande: Look")
        self.network.send_command("Look\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        if response == "dead":
            return "dead"
        if response == "ko":
            return []
        return self._parse_look(response)

    def _parse_look(self, raw_str):
        parsed_vision = []
        try:
            clean_str = raw_str.strip('[]\n ')
            if not clean_str:
                return []
            tiles = clean_str.split(',')
            for tile in tiles:
                parsed_vision.append(tile.strip().split())
        except Exception as e:
            print(f"[Erreur] Parsing vision échoué sur '{raw_str}': {e}")
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
        if response == "dead":
            return "dead"
        if response == "ko":
            return {}
        return self._parse_inventory(response)
    
    def _parse_inventory(self, raw_str):
        inventory = {}
        try:
            clean_str = raw_str.strip('[]\n ')
            if not clean_str:
                return {}
            items = clean_str.split(',')
            for item in items:
                parts = item.strip().split()
                if len(parts) == 2:
                    inventory[parts[0]] = int(parts[1])
        except Exception:
            pass
        return inventory
    
    def forward(self):
        print("[Agent] commande: Forward")
        self.network.send_command("Forward\n")
        return self._wait_for_response() == "ok"
    
    def right(self):
        print("[Agent] commande: Right")
        self.network.send_command("Right\n")
        return self._wait_for_response() == "ok"
    
    def left(self):
        print("[Agent] commande: Left")
        self.network.send_command("Left\n")
        return self._wait_for_response() == "ok"
    
    def take_object(self, obj):
        print(f"[Agent] commande: Take {obj}")
        self.network.send_command(f"Take {obj}\n")
        return self._wait_for_response() == "ok"
    
    def set_object(self, obj):
        print(f"[Agent] commande: Set {obj}")
        self.network.send_command(f"Set {obj}\n")
        return self._wait_for_response() == "ok"

    def incantation(self):
        print("[Agent] commande: Incantation")
        self.network.send_command("Incantation\n")
        first_response = self._wait_for_response(expect_incantation=True)
        print(f"[Agent] Incantation (Début): {first_response}")
        if first_response == "ko":
            return False
        if first_response == "Elevation underway":
            second_response = self._wait_for_response(expect_incantation=True)
            print(f"[Agent] Incantation (Fin): {second_response}")
            return second_response.startswith("Current level")
            
        return False

    def connect_nbr(self):
        print("[Agent] commande: Connect_nbr")
        self.network.send_command("Connect_nbr\n")
        response = self._wait_for_response()
        return -1 if response == "ko" else int(response)
    
    def fork(self):
        print("[Agent] commande: Fork")
        self.network.send_command("Fork\n")
        return self._wait_for_response() == "ok"

    def eject(self):
        print("[Agent] commande: Eject")
        self.network.send_command("Eject\n")
        return self._wait_for_response() == "ok"
    
    def death(self):
        print("[Agent] commande: Death")
        self.network.send_command("Death\n")
        return self._wait_for_response() == "dead"
