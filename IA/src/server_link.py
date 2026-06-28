#!/usr/bin/env python3

import socket
import select
from collections import deque
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("server_link")

CIRCULAR_BUFFER_MAX_SIZE = 65536

class CircularBuffer:
    def __init__(self, max_size=CIRCULAR_BUFFER_MAX_SIZE):
        self._buffer = deque(maxlen=max_size) # Buffer circulaire à taille fixe.
        self._max_size = max_size # Taille maximale du buffer.

    def write(self, data: str):
        """Écrit des données dans le buffer circulaire. Si le buffer dépasse sa taille max, les caractères les plus anciens sont supprimés.
        Params:
            -data (str): Les données à écrire dans le buffer.
        Ne retourne rien."""
        for char in data:
            self._buffer.append(char)

    def extract_messages(self) -> list:
        """Extrait tous les messages complets (délimités par '\\n') du buffer. Les messages incomplets (sans '\\n' final) restent dans le buffer.
        Params:
            Aucun paramètre.
        Retourne une liste de messages complets (str)."""
        content = ''.join(self._buffer)
        messages = []
        while '\n' in content:
            msg, content = content.split('\n', 1)
            msg = msg.strip()
            if msg:
                messages.append(msg)
        self._buffer.clear()
        for char in content:
            self._buffer.append(char)
        return messages

    def has_complete_message(self) -> bool:
        """Vérifie si le buffer contient au moins un message complet (terminé par '\\n').
        Params:
            Aucun paramètre.
        Retourne True si un message complet est présent, False sinon."""
        return '\n' in ''.join(self._buffer)

    def __len__(self):
        return len(self._buffer)


class ZappyNetwork:
    def __init__(self, machine, port):
        self.machine = machine # Adresse du serveur Zappy (IP ou hostname).
        self.port = port # Port de connexion au serveur Zappy.
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Socket TCP pour communiquer avec le serveur.
        self.buffer = CircularBuffer() # Buffer circulaire pour stocker les données reçues du serveur avant de les parser en messages complets.
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
        readable, _, _ = select.select([self.sock], [], [], 0)
        if readable:
            try:
                data = self.sock.recv(4096).decode('utf-8')
                if not data:
                    logger.warning(f"Le serveur {self.machine}:{self.port} a fermé la connexion.")
                    self.connected = False
                    return []
                self.buffer.write(data)
            except BlockingIOError:
                pass
            except socket.error as e:
                logger.error(f"Erreur de réception des messages du serveur {self.machine}:{self.port}: {e}")
                self.connected = False
                return []
        return self.buffer.extract_messages()

    def close(self):
        """Ferme la connexion au serveur Zappy.\n
        Params:
            aucun paramètre.
        Retourne None."""

        if self.connected:
            self.sock.close()
            self.connected = False
