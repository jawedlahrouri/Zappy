import random 

class BroadcastMessage:
    def __init__(self, command_interface):
        self.cmd = command_interface

    def broadcast_text(self, message):
        self.cmd.broadcast_text(message)
        