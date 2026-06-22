#!/usr/bin/env python3

import socket
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("server_link")

class ZappyNetwork:
    def __init__(self, machine, port):
        self.machine = machine # Adresse du serveur Zappy (IP ou hostname).
        self.port = port # Port de connexion au serveur Zappy.
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Socket TCP pour communiquer avec le serveur.
        self.buffer = "" # Buffer pour stocker les données reçues du serveur avant de les parser en messages complets.
        self.connected = False # Flag pour indiquer si la connexion au serveur est établie.

    def connect(self):
        """Établit une connexion au serveur Zappy en utilisant les paramètres fournis lors de l'initialisation.\n
        Params:
            aucun paramètre.
        Retourne None."""
        try:
            self.sock.connect((self.machine, self.port))
            self.sock.setblocking(False)
            self.connected = True
            logger.info(f"Connecté au serveur {self.machine}:{self.port}")
        except ConnectionRefusedError:
            logger.error(f"Connexion refusée par le serveur {self.machine}:{self.port}. Vérifiez que le serveur est en ligne et que les paramètres sont corrects.")
            self.connected = False
            raise
        except socket.timeout:
            logger.error(f"Timeout de connexion à {self.machine}:{self.port}. Vérifiez la connectivité réseau et que le serveur est en ligne.")
            self.connected = False
            raise

    def send_command(self, command: str):
        """Envoie une commande au serveur Zappy.\n
        Params:
            -command (str): La commande à envoyer au serveur.
        Retourne True si la commande est envoyée avec succès, False sinon."""

        if not self.connected:
            logger.error("[Erreur] Non connecté au serveur")
            return False
        if not command.endswith('\n'):
            command += '\n'        
        try:
            self.sock.sendall(command.encode('utf-8'))
            return True
        except socket.error as e:
            logger.error(f"Erreur d'envoi de la commande {command} à {self.machine}:{self.port}: {e}")
            self.connected = False
            return False

    def receive_messages(self) -> list:
        """Reçoit les messages du serveur Zappy et les parse en une liste de messages complets.\n
        Params:
            aucun paramètre.
        Retourne une liste de messages reçus du serveur."""

        if not self.connected:
            return []
        try:
            data = self.sock.recv(4096).decode('utf-8')
            if not data:
                logger.warning(f"Le serveur {self.machine}:{self.port} a fermé la connexion.")
                self.connected = False
                return []
            self.buffer += data
        except BlockingIOError:
            pass
        except socket.error as e:
            logger.error(f"Erreur de réception des messages du serveur {self.machine}:{self.port}: {e}")
            self.connected = False
            return []
        messages = []
        while '\n' in self.buffer:
            msg, self.buffer = self.buffer.split('\n', 1)
            msg = msg.strip()
            if msg:
                messages.append(msg)
        return messages

    def close(self):
        """Ferme la connexion au serveur Zappy.\n
        Params:
            aucun paramètre.
        Retourne None."""

        if self.connected:
            self.sock.close()
            self.connected = False
