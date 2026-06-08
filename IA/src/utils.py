#!/usr/bin/python3

import random
import time
import sys
import subprocess
from enum import Enum
from src.command import ZappyCommand
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms

class utils:
    def __init__(self, command_interface: ZappyCommand, team_name: str):
        self.cmd = command_interface
        self.pathfinder = Pathfinder(self.cmd)
        self.farmer = Farmer()
        self.comms = StealthComms()
        self.grouper = Grouper(self.cmd, self.comms)

    def  launch_clone(self):
        print("[Agent] Un œuf a été pondu.")
        print("[Agent] Lancement d'un nouveau clone en arrière-plan...")
        port = str(self.cmd.network.port)
        machine = self.cmd.network.machine
        subprocess.Popen([sys.executable, "main.py", "-p", port, "-h", machine, "-n", self.team_name])


