#!/usr/bin/python3

import select
from src.server_link import ZappyNetwork
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("ZappyCommand")

class ZappyCommand:
    def __init__(self, network):
        self.network = network # L'instance de connexion réseau pour envoyer les commandes et recevoir les réponses.
        self.broadcast_queue = [] # Queue pour stocker les messages de diffusion reçus du serveur, à traiter par l'agent.
        self.system_queue = [] # Queue pour stocker les messages système (comme les réponses aux commandes) reçus du serveur, à traiter par l'agent.

    def wait_for_incantation_follower(self):
        """Attends les messages d'incantation pour un follower, en gérant les réponses inattendues.\n
        Params:
            -Aucun paramètre.
        Retourne True si l'incantation réussit, False en cas d'échec."""

        logger.info(f"l'agent attend les messages d'incantation.")
        timeout_counter = 0
        self.network.send_command("Inventory\n")
        
        while True:
            response = self._wait_for_response(expect_incantation=True)
            if response == "Elevation underway":
                second_response = self._wait_for_response(expect_incantation=True)
                logger.info(f"l'agent a reçu les messages d'incantation du leader. Résultat: {second_response}")
                if second_response and second_response.startswith("Current level"):
                    self._wait_for_response() 
                    return True
                return False
            elif response.startswith("["): 
                timeout_counter += 1
                if timeout_counter > 800:
                    logger.warning("Le leader n'a pas lancé l'incantation à temps, annulation de l'attente FOLLOWER.")
                    return False
                self.network.send_command("Inventory\n")
            elif response == "ko":
                return False

    def _wait_for_response(self, expect_incantation=False):
        """Attends une réponse du serveur, en filtrant les messages de diffusion et en gérant les cas d'incantation.\n
        Params:
            -expect_incantation (bool): Indique si on s'attend à une réponse d'incantation.
        Retourne la réponse reçue."""

        while True:
            if self.system_queue:
                msg = self.system_queue.pop(0)
                if not expect_incantation:
                    if msg == "Elevation underway" or msg.startswith("Current level"):
                        continue
                return msg
            messages = self.network.receive_messages()
            for msg in messages:
                if msg.startswith("message "):
                    self.broadcast_queue.append(msg)
                elif msg.startswith("eject: "):
                    logger.warning(f"l'agent a été éjecté par un autre joueur. Détails: {msg}")
                elif msg == "dead":
                    self.system_queue.append("dead")
                else:
                    self.system_queue.append(msg)
            if not messages:
                select.select([self.network.sock], [], [])
    
    def get_unread_broadcasts(self):
        """Récupère les messages de diffusion non lus et les vide de la queue.\n
        Params:
            -Aucun paramètre.
        Retourne une liste des messages de diffusion non lus."""

        unread = self.broadcast_queue.copy()
        self.broadcast_queue.clear()
        return unread


    def _parse_look(self, raw_str):
        """Parse la réponse de la commande Look pour extraire la vision de l'agent.\n
        Params:
            -raw_str (str): La chaîne de caractères brute de la réponse.
        Retourne une liste des tuiles dans la vision."""

        parsed_vision = []
        try:
            clean_str = raw_str.strip('[]\n ')
            if not clean_str:
                return []
            tiles = clean_str.split(',')
            for tile in tiles:
                parsed_vision.append(tile.strip().split())
        except Exception as e:
            logger.error(f"[Erreur] Parsing vision échoué sur '{raw_str}': {e}")
        return parsed_vision
    
    def _parse_inventory(self, raw_str):
        """Parse la réponse de la commande Inventory pour extraire l'inventaire de l'agent.\n
        Params:
            -raw_str (str): La chaîne de caractères brute de la réponse.
        Retourne un dictionnaire des objets dans l'inventaire."""

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

    def incantation(self):
        """Envoi la commande Incantation au serveur et attends la réponse, en gérant les cas de succès, d'échec, et de refus.\n
        Params:
            -Aucun paramètre.
        Retourne True si l'incantation est un succès, False sinon."""
        logger.info(f"l'agent envoie la commande: Incantation")
        self.network.send_command("Incantation\n")
        first_response = self._wait_for_response(expect_incantation=True)
        logger.info(f"l'agent envoie la commande: {first_response}")
        if first_response == "ko":
            return False
        if first_response == "Elevation underway":
            second_response = self._wait_for_response(expect_incantation=True)
            logger.info(f"l'agent envoie la commande: {second_response}")
            return second_response.startswith("Current level")
        return False

    def connect_nbr(self):
        """Envoie la commande Connect_nbr au serveur pour connaître le nombre de connexions disponibles, et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne le nombre de connexions disponibles, ou -1 en cas de refus."""

        logger.info(f"l'agent envoie la commande:  Connect_nbr")
        self.network.send_command("Connect_nbr\n")
        response = self._wait_for_response()
        return -1 if response == "ko" else int(response)
    
    def death(self):
        """Envoie la commande Death au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        self.network.send_command("Death\n")
        logger.warning(f"l'agent envoie la commande: death")
        return self._wait_for_response() == "dead"

    def send_command(self, command):
        """Envoie la commande passé en parametres au serveur
        Params:
            -command (str): La command à envoyé au server
        Retourne la reponse du serveur"""

        if not command.endswith('\n'):
            command += '\n' 
        cmd_name = command.split(' ')[0].strip() 
        logger.info(f"L'agent envoie la commande : {command.strip()}")
        if cmd_name == "Connect_nbr":
            return self.connect_nbr()
        if cmd_name == "Incantation":
            return self.incantation()
        self.network.send_command(command)
        response = self._wait_for_response()
        logger.info(f"L'agent a reçu la réponse : {response}")
        if response == "dead":
            return "dead"
        parsers = {
            "Look": lambda r: [] if r == "ko" else self._parse_look(r),
            "Inventory": lambda r: {} if r == "ko" else self._parse_inventory(r)
        }
        if cmd_name in parsers:
            return parsers[cmd_name](response)
        return response == "ok"
