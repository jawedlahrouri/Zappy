#!/usr/bin/python3

import random
import sys
import subprocess
from enum import Enum
from src.command import ZappyCommand
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms
from src.logger import ZappyLogger
from src.Ai_agent import ZappyAgent

logger = ZappyLogger.get_logger("AgentCore")

class utils:
    def __init__(self, command_interface: ZappyCommand, team_name: str):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms) # Module de groupage contenant la logique de regroupement de notre agent.
        self.team_name = team_name # Le nom de l'équipe, utilisé pour le lancement de clones et la communication.
        self.agent = ZappyAgent() # agent de zappy

    def  launch_clone(self):
        """Lance un nouveau clone de l'agent en arrière-plan en utilisant subprocess pour exécuter une nouvelle instance du client Zappy.\n
        Params:
            aucun paramètre.
        Retourne None."""
        
        logger.info(f"l'agent a pondu un nouvel œuf et lance un nouveau clone.")
        port = str(self.cmd.network.port)
        machine = self.cmd.network.machine
        subprocess.Popen(["./zappy_ai", "-p", port, "-h", machine, "-n", self.team_name])

    def is_food_critical_for_action(self, estimated_tu_cost, inventory):
        """Vérifie si la nourriture actuelle est insuffisante pour effectuer une action risquée\n
        Params:
            -estimated_tu_cost (int): Le coût en TU estimé de l'action risquée que l'on veut faire.
        Retourne True si la nourriture est insuffisante, False sinon."""

        food = inventory.get('food', 0)
        current_tu = food * 126
        if current_tu > estimated_tu_cost:
            return True
        return False

    def fork(self):
        success = self.agent.cmd.send_command("Fork")
        if success:
            logger.info("Reine : Un oeuf a été pondu. En attente de l'éclosion (600 TU)...")
            self.agent.eggs_laid += 1
            return True
        return False
            
    def verify_command(self, command, commands):  # message_stack = {id : [(k, (command, [params])), (k, (command, [params]))], ... }
        for tupple in commands:
            if tupple[1][0] == command:
                return tupple[1][1]
            else:
                continue
        return None

    def verify_instruction(self, command, commands):  # message_stack = {id : [(k, (command, [params])), (k, (command, [params]))], ... }
        for tupple in commands:
            if tupple[1][0] == command:
                return True
            else:
                continue
        return False
    
    def verify_id_message(self, id):
        return self.agent.message_stack[id]
    
    def which_role(self):
        messages = self.verify_id_message(9999)
        params = self.verify_command("royalsummon", messages)
        if self.is_queen == True:
            return 1
        for e in params:
            if self.agent.agent_id == e and e <= 5:
                return 2
            if self.agent.agent_id == e and e > 5:
                return 3
        return 4
    
    def is_grouping(self):
        messages = self.verify_id_message(9999)
        return self.verify_instruction("grouping", messages)
    
    def is_elevation(self):
        messages = self.verify_id_message(9999)
        return self.verify_instruction("elevation", messages)