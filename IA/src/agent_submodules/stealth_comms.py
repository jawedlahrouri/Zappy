import random
import string

class StealthComms:
    def __init__(self):
        self.previous_calls = set()

    def _generate_stealth_token(self, length=14):
        chars = string.ascii_letters
        base_token = ''.join(random.choices(chars, k=length - 1))
        ascii_sum = sum(ord(c) for c in base_token)
        checksum_char = chars[ascii_sum % len(chars)]
        return base_token + checksum_char

    def is_token_valid(self, token):
        if len(token) != 14:
            return False
        chars = string.ascii_letters
        base_token = token[:-1]
        expected_checksum = token[-1]
        ascii_sum = sum(ord(c) for c in base_token)
        calculated_checksum = chars[ascii_sum % len(chars)]
        return expected_checksum == calculated_checksum

    def encode(self, param, command):
        token = self._generate_stealth_token()
        return f"{token}_{param}_{command}"

    def decode(self, raw_text):
        parts = raw_text.split('_')
        if len(parts) != 3:
            return None
        token, param, command = parts
        if not self.is_token_valid(token):
            return None
        if token in self.previous_calls:
            print("[Comms] Tentative de rejeu ennemie détectée et esquivée !")
            return None
        self.previous_calls.add(token)
        return {"command": command, "param": param}