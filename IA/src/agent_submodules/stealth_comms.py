#!/usr/bin/python3
import random
import string
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("StealthComms")

class StealthComms:
    def __init__(self):
        self.previous_calls = set() # Set pour stocker les tokens déjà vus et éviter les répétitions.

    def _generate_stealth_token(self, length=14):
        """Génère un token de communication furtif de longueur spécifiée, avec une somme de caractères vérifiable pour éviter les faux messages.\n
        Params:
            -length (int): La longueur du token à générer.
        Retourne:
            -str: Le token généré."""

        chars = string.ascii_letters
        base_token = ''.join(random.choices(chars, k=length - 1))
        ascii_sum = sum(ord(c) for c in base_token)
        checksum_char = chars[ascii_sum % len(chars)]
        return base_token + checksum_char

    def is_token_valid(self, token):
        """Vérifie si un token de communication furtif est valide en vérifiant sa longueur et son checksum.\n
        Params:
            -token (str): Le token à vérifier.
        Retourne:
            -bool: True si le token est valide, False sinon."""

        if len(token) != 14:
            return False
        chars = string.ascii_letters
        base_token = token[:-1]
        expected_checksum = token[-1]
        ascii_sum = sum(ord(c) for c in base_token)
        calculated_checksum = chars[ascii_sum % len(chars)]
        return expected_checksum == calculated_checksum

    def encode(self, param, command):
        """Encode une commande de communication furtive avec un paramètre donné, en générant un token unique pour chaque message.\n
        Params:
            -param (str): Le paramètre à encoder.
            -command (str): La commande à encoder.
        Retourne:
            -str: Le message encodé."""

        token = self._generate_stealth_token()
        return f"{token}_{param}_{command}"

    def decode(self, raw_text):
        """Décode un message de communication furtif en extrayant le token, le paramètre et la commande.\n
        Params:
            -raw_text (str): Le message brut à décoder.
        Retourne:
            -dict or None: Un dictionnaire contenant les éléments décodés ou None en cas d'erreur."""

        parts = raw_text.split('_', 2)
        if len(parts) != 3:
            return None
        token, param, command = parts
        if not self.is_token_valid(token):
            return None
        if token in self.previous_calls:
            return None
        self.previous_calls.add(token)
        return {"command": command, "param": param}
