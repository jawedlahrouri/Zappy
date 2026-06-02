#!/usr/bin/python3

import time
from src.server_link import ZappyNetwork

class ZappyCommand:
    def __init__(self, network):
        self.network = network

    def look(self):
        print("[Agent] commande: Look")
        self.network.send_command("Look\n")
        response = self._wait_for_response()
        print(f"[Agent] Réponse: {response}")
        if response == "ko":
            return []
        return self._parse_look(response)

    def _parse_look(self, str):
        parsed_vision = []
        clean_str = str.strip('[]\n')
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
