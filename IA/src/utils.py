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
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentCore")

class utils:
    def __init__(self, command_interface: ZappyCommand, team_name: str):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms) # Module de groupage contenant la logique de regroupement de notre agent.
        self.team_name = team_name # Le nom de l'équipe, utilisé pour le lancement de clones et la communication.

    def  launch_clone(self):
        """Lance un nouveau clone de l'agent en arrière-plan en utilisant subprocess pour exécuter une nouvelle instance du client Zappy.\n
        Params:
            aucun paramètre.
        Retourne None."""
        
        logger.info(f"l'agent {self.agent_id} a pondu un nouvel œuf et lance un nouveau clone.")
        print("[Agent] Un œuf a été pondu.")
        print("[Agent] Lancement d'un nouveau clone en arrière-plan...")
        port = str(self.cmd.network.port)
        machine = self.cmd.network.machine
        subprocess.Popen(["./zappy_ai", "-p", port, "-h", machine, "-n", self.team_name])

    def is_food_critical_for_action(self, estimated_tu_cost):
        """Vérifie si la nourriture actuelle est insuffisante pour effectuer une action risquée\n
        Params:
            -estimated_tu_cost (int): Le coût en TU estimé de l'action risquée que l'on veut faire.
        Retourne True si la nourriture est insuffisante, False sinon."""

        food = self.inventory.get('food', 0)
        current_tu = food * 126
        minimum_tu_required = (self.food_minimum * 126)
        if current_tu < (estimated_tu_cost + minimum_tu_required):
            return True
        return False
