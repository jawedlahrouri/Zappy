#!/usr/bin/env python3

import socket
import time

class ZappyNetwork:
    def __init__(self, machine, port):
        self.machine = machine
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.buffer = ""
        self.connected = False

    #se connecte au serveur
    def connect(self):
        try:
            self.sock.connect((self.machine, self.port))
            self.sock.setblocking(False)
            self.connected = True
            print(f"[Connexion] Connecté au serveur {self.machine}:{self.port}")
        except ConnectionRefusedError:
            print(f"[Erreur] Impossible de se connecter à {self.machine}:{self.port}")
            self.connected = False
            raise
        except socket.timeout:
            print(f"[Erreur] Timeout de connexion à {self.machine}:{self.port}")
            self.connected = False
            raise

    #envoie une commande au serveur
    def send_command(self, command: str):
        if not self.connected:
            print("[Erreur] Non connecté au serveur")
            return False
        if not command.endswith('\n'):
            command += '\n'        
        try:
            self.sock.sendall(command.encode('utf-8'))
            return True
        except socket.error as e:
            print(f"[Erreur d'envoi] {e}")
            self.connected = False
            return False

    #reçoit les messages du serveur et les parse en une liste de messages complet
    def receive_messages(self) -> list:
        if not self.connected:
            return []
        try:
            data = self.sock.recv(4096).decode('utf-8')
            if not data:
                print("[Avertissement] Serveur a fermé la connexion")
                self.connected = False
                return []
            self.buffer += data
        except BlockingIOError:
            pass
        except socket.error as e:
            print(f"[Erreur de réception] {e}")
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
        if self.connected:
            self.sock.close()
            self.connected = False
