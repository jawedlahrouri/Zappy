#!/usr/bin/python3

import time
from src.server_link import ZappyNetwork
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("ZappyCommand")

class ZappyCommand:
    def __init__(self, network):
        self.network = network # L'instance de connexion réseau pour envoyer les commandes et recevoir les réponses.
        self.broadcast_queue = [] # Queue pour stocker les messages de diffusion reçus du serveur, à traiter par l'agent.
        self.system_queue = [] # Queue pour stocker les messages système (comme les réponses aux commandes) reçus du serveur, à traiter par l'agent.

    def wait_for_incantation_follower(self):
        """Attends les messages d'incantation pour un follower, en gérant les cas de timeout et de réponses inattendues.\n
        Params:
            -Aucun paramètre.
        Retourne True si l'incantation réussit, False en cas d'échec ou de timeout."""

        logger.info(f"l'agent attend les messages d'incantation.")
        start_time = time.time()
        first_response = None
        
        while time.time() - start_time < 10.0:
            first_response = self._wait_for_response(expect_incantation=True, timeout=1.0)
            if first_response in ["Elevation underway", "ko"]:
                break     
        if first_response == "Elevation underway":
            second_response = self._wait_for_response(expect_incantation=True, timeout=300.0)
            logger.info(f"l'agent a reçu les messages d'incantation du leader. Résultat: {second_response}")
            if second_response and second_response.startswith("Current level"):
                return True
        return False

    def _wait_for_response(self, expect_incantation=False, timeout=None):
        """Attends une réponse du serveur, en filtrant les messages de diffusion et en gérant les cas d'incantation.\n
        Params:
            -expect_incantation (bool): Indique si on s'attend à une réponse d'incantation.
            -timeout (float): Le délai maximum d'attente en secondes.
        Retourne la réponse reçue ou "timeout" en cas de dépassement du délai."""

        start_time = time.time()
        while True:
            if timeout and (time.time() - start_time > timeout):
                return "timeout"
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
                    logger.warning(f"l'agent est mort. Il etait niveau {self.level}. Agent restant: {len(self.team_roster)}.")
                else:
                    self.system_queue.append(msg)
            time.sleep(0.05)
    
    def get_unread_broadcasts(self):
        """Récupère les messages de diffusion non lus et les vide de la queue.\n
        Params:
            -Aucun paramètre.
        Retourne une liste des messages de diffusion non lus."""

        unread = self.broadcast_queue.copy()
        self.broadcast_queue.clear()
        return unread

    def look(self):
        """Envoie la commande Look au serveur et attend la réponse, en gérant les cas de mort et de refus.\n
        Params:
            -Aucun paramètre.
        Retourne la vision analysée ou "dead" si l'agent est mort, ou une liste vide en cas de refus."""

        logger.info(f"l'agent envoie la commande: Look")
        self.network.send_command("Look\n")
        response = self._wait_for_response()
        logger.info(f"l'agent envoie la commande: {response}")
        if response == "dead":
            return "dead"
        if response == "ko":
            return []
        return self._parse_look(response)

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
    
    def broadcast_text(self, text):
        """Envoie un message de diffusion au serveur et attend la confirmation.\n
        Params:
            -text (str): Le texte à diffuser.
        Retourne True si la diffusion est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande: Broadcast '{text}'")
        self.network.send_command(f"Broadcast {text}\n")
        response = self._wait_for_response()
        logger.info(f"l'agent envoie la commande: {response}")
        return response == "ok"
    
    def inventory(self):
        """Envoie la commande Inventory au serveur et attend la réponse, en gérant les cas de mort et de refus.\n
        Params:
            -Aucun paramètre.
        Retourne l'Inventaire analysée ou "dead" si l'agent est mort, ou une liste vide en cas de refus."""

        logger.info(f"l'agent envoie la commande: Inventory")
        self.network.send_command("Inventory\n")
        response = self._wait_for_response()
        logger.info(f"l'agent envoie la commande: {response}")
        if response == "dead":
            return "dead"
        if response == "ko":
            return {}
        return self._parse_inventory(response)
    
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
    
    def forward(self):
        """Envoie la commande Forward au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande: Forward")
        self.network.send_command("Forward\n")
        return self._wait_for_response() == "ok"
    
    def right(self):
        """Envoie la commande Right au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande: Right")
        self.network.send_command("Right\n")
        return self._wait_for_response() == "ok"
    
    def left(self):
        """Envoie la commande Left au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande: Left")
        self.network.send_command("Left\n")
        return self._wait_for_response() == "ok"
    
    def take_object(self, obj):
        """Envoie la commande Take pour un objet spécifique au serveur et attend la réponse.\n
        Params:
            -obj (str): Le nom de l'objet à prendre.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande:  Take {obj}")
        self.network.send_command(f"Take {obj}\n")
        return self._wait_for_response() == "ok"
    
    def set_object(self, obj):
        """Envoie la commande Set pour un objet spécifique au serveur et attend la réponse.\n
        Params:
            -obj (str): Le nom de l'objet à poser.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande:  Set {obj}")
        self.network.send_command(f"Set {obj}\n")
        return self._wait_for_response() == "ok"

    def connect_nbr(self):
        """Envoie la commande Connect_nbr au serveur pour connaître le nombre de connexions disponibles, et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne le nombre de connexions disponibles, ou -1 en cas de refus."""

        logger.info(f"l'agent envoie la commande:  Connect_nbr")
        self.network.send_command("Connect_nbr\n")
        response = self._wait_for_response()
        return -1 if response == "ko" else int(response)
    
    def fork(self):
        """Envoie la commande Fork au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        logger.info(f"l'agent envoie la commande: Fork")
        self.network.send_command("Fork\n")
        return self._wait_for_response() == "ok"

    def eject(self):
        """Envoie la commande Eject au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        self.network.send_command("Eject\n")
        return self._wait_for_response() == "ok"
    
    def death(self):
        """Envoie la commande Death au serveur et attend la réponse.\n
        Params:
            -Aucun paramètre.
        Retourne True si la commande est confirmée, False sinon."""

        self.network.send_command("Death\n")
        logger.warning(f"l'agent envoie la commande: death")
        return self._wait_for_response() == "dead"
