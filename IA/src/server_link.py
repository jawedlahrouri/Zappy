#!/usr/bin/env python3

import socket

class ZappyNetwork:
    def __init__(self, machine, port):
        self.machine = machine
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.buffer = ""
        self.connected = False

    def connect(self):
        try:
            self.sock.connect((self.machine, self.port))
            self.connected = True
            print(f"Connecté au serveur {self.machine}:{self.port}")
        except ConnectionRefusedError:
            print("Erreur: Impossible de se connecter au serveur.")
            exit(84)

    def send_command(self, command: str):
        if not self.connected:
            return
        if not command.endswith('\n'):
            command += '\n'
        try:
            self.sock.sendall(command.encode('utf-8'))
        except socket.error as e:
            print(f"Erreur d'envoi: {e}")
            self.connected = False

    def receive_messages(self) -> list:
        if not self.connected:
            return []
        try:
            data = self.sock.recv(4096).decode('utf-8')
            if not data:
                print("Le serveur a fermé la connexion.")
                self.connected = False
                return []
            self.buffer += data
        except socket.error as e:
            print(f"Erreur de réception: {e}")
            self.connected = False
            return []
        messages = []
        while '\n' in self.buffer:
            msg, self.buffer = self.buffer.split('\n', 1)
            messages.append(msg)
            
        return messages

    def close(self):
        if self.connected:
            self.sock.close()
            self.connected = False