#!/usr/bin/python3

import subprocess
from src.agent_submodules.agent_role import AgentRole
from src.agent_submodules.agent_state import AgentState
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentCore")

class utils:
    def __init__(self, command_interface, team_name: str, agent):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.team_name = team_name # Le nom de l'équipe, utilisé pour le lancement de clones et la communication.
        self.agent = agent # Référence à l'agent principal.

    def launch_clone(self):
        """Lance un nouveau clone de l'agent en arrière-plan en utilisant subprocess pour exécuter une nouvelle instance du client Zappy.\n
        Params:
            aucun paramètre.
        Retourne None."""
        
        logger.info(f"l'agent a pondu un nouvel œuf et lance un nouveau clone.")
        port = str(self.cmd.network.port)
        machine = self.cmd.network.machine
        subprocess.Popen(["./zappy_ai", "-p", port, "-h", machine, "-n", self.team_name])

    def is_food_critical_for_action(self, estimated_tu_cost, inventory):
        """Vérifie si la nourriture actuelle est suffisante pour effectuer une action risquée\n
        Params:
            -estimated_tu_cost (int): Le coût en TU estimé de l'action risquée que l'on veut faire.
            -inventory (dict): L'inventaire de l'agent.
        Retourne True si la nourriture est suffisante, False sinon."""

        food = inventory.get('food', 0)
        current_tu = food * 126
        if current_tu > estimated_tu_cost:
            return True
        return False

    def fork(self):
        """Pond un oeuf via la commande Fork.\n
        Retourne True si réussi, False sinon."""
        success = self.agent.cmd.send_command("Fork")
        if success:
            logger.info("Reine : Un oeuf a été pondu. En attente de l'éclosion (600 TU)...")
            self.agent.eggs_laid += 1
            return True
        return False
            
    def verify_command(self, command, commands):
        """Cherche une commande dans la liste de messages et retourne ses params.
        commands est une liste de dicts : [{k: (command, [params])}, ...]
        Retourne les params si trouvé, None sinon."""
        for msg_dict in commands:
            for k, (cmd, params) in msg_dict.items():
                if cmd == command:
                    return params
        return None

    def verify_instruction(self, command, commands):
        """Vérifie si une commande existe dans la liste de messages.
        Retourne True si trouvé, False sinon."""
        for msg_dict in commands:
            for k, (cmd, params) in msg_dict.items():
                if cmd == command:
                    return True
        return False
    
    def verify_id_message(self, id):
        """Retourne les messages d'un agent spécifique, ou None si absent."""
        return self.agent.message_stack.get(id, None)
    
    def which_role(self):
        """Détermine le rôle de l'agent en fonction du décret royal de la reine.
        Retourne le rôle approprié (AgentRole)."""
        if self.agent.is_queen:
            return AgentRole.QUEEN
        messages = self.verify_id_message(9999)
        if messages is None:
            return self.agent.role  # Pas de message de la reine, on garde le rôle actuel.
        params = self.verify_command("royalsummon", messages)
        if params is None:
            return self.agent.role
        # params contient les IDs des guards (indices 0-4) et protectors (indices 5-8)
        agent_id_str = str(self.agent.agent_id)
        for i, e in enumerate(params):
            if agent_id_str == e and i < 5:
                return AgentRole.GUARD
            if agent_id_str == e and i >= 5:
                return AgentRole.PROTECTOR
        return AgentRole.PEASANT
    
    def is_grouping(self):
        """Vérifie si la reine a envoyé un ordre de grouping."""
        messages = self.verify_id_message(9999)
        if messages is None:
            return False
        return self.verify_instruction("grouping", messages)
    
    def is_elevation(self):
        """Vérifie si la reine a envoyé un ordre d'élévation."""
        messages = self.verify_id_message(9999)
        if messages is None:
            return False
        # Stocker le k de la reine pour savoir dans quelle direction aller
        for msg_dict in messages:
            for k, (cmd, params) in msg_dict.items():
                if cmd == "elevation":
                    self.agent.k_queen = k
                    return True
        return False

    def verify_food(self):
        """Vérifie si l'agent doit ramasser de la nourriture.
        Retourne True si il doit en ramasser, False sinon."""
        food_level = self.agent.inventory.get("food", 0)
        if self.agent.state == AgentState.GROUPING:
            return food_level <= 25
        if self.agent.state == AgentState.ELEVATION:
            return food_level <= 15
        return food_level <= 6
