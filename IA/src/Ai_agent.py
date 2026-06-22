#!/usr/bin/python3

import random
import subprocess
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms
from src.agent_submodules.agent_state import StateComportment, AgentState
from src.agent_submodules.agent_role import RoleTask, AgentRole
from src.command import ZappyCommand
from src.utils import utils
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentCore")

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand, team_name: str, map_size, client_num):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms, self) # Module de groupage contenant la logique de regroupement de notre agent.
        self.utils = utils(self.cmd, team_name, self) # Utilitaires divers, comme le lancement de clones.
        self.comportment = StateComportment(self) # Comportement de l'agent selon son état (survie, farming, grouping, etc).
        self.task = RoleTask(self) # Role de l'agent

        self.client_num = client_num # Le nombre de slots clients attribués à notre équipe, utilisé pour décider si on doit faire du forking dès le départ.
        self.team_name = team_name # Le nom de l'équipe.
        self.map_size = map_size # Dimensions de la carte.
        self.unread_messages = [] # Messages reçus du serveur mais pas encore traités.
        self.agent_id = random.randint(1000, 9998) # Un ID unique pour cet agent, utilisé dans les communications cryptées.
        self.is_alive = True # Flag pour savoir si l'agent est encore en vie.
        self.state = AgentState.BEGINNER # État initial de l'agent, il commencera en mode beginner.
        self.role = AgentRole.PEASANT
        self.level = 1 # Niveau actuel de l'agent, commence à 1 et augmente avec les incantations réussies.
        self.inventory = {} # Dictionnaire pour stocker les ressources que l'agent a en sa possession (nourriture, pierres, etc).
        self.vision = [] # Ce que l'agent voit autour de lui, mis à jour à chaque cycle.
        self.food_minimum = 6 # Seuil de nourriture en dessous duquel l'agent passe en mode survie pour éviter de mourir de faim.
        self.food_safe = 15 # Seuil de nourriture au-dessus duquel l'agent se sent en sécurité.
        self.team_roster = {} # Dictionnaire pour suivre les alliés connus et leur dernière activité.
        self.population_target = 15 # Nombre d'agents que nous voulons idéalement dans l'équipe.
        self.is_queen_elected = False # Flag pour savoir si la reine a été élue.
        self.is_queen = False # Flag pour savoir si cet agent est la reine.
        self.eggs_laid = 0 # Nombre d'oeufs pondus par la reine.
        self.eggs_laid = 0 # Nombre d'oeufs pondus par la reine.
        self.message_stack = {} # Messages radio parsés du cycle courant.

    def update_internal_state(self):
        """Met à jour la vision, l'inventaire, et les messages reçus du serveur.
        Vérifie aussi la connexion et la survie de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        response = self.cmd.send_command('Inventory')
        if response == 'dead':
            self.is_alive = False
            return
        self.inventory = response
        response = self.cmd.send_command('Look')
        if response == 'dead':
            self.is_alive = False
            return
        self.vision = response
        if not (self.role == AgentRole.PEASANT and self.state in [AgentState.GROUPING, AgentState.ELEVATION]):
            ping_content = f"{self.agent_id}-{self.level}-{self.inventory.get('food', 0)}-{self.inventory.get('linemate', 0)}-{self.inventory.get('deraumere', 0)}-{self.inventory.get('sibur', 0)}-{self.inventory.get('mendiane', 0)}-{self.inventory.get('phiras', 0)}-{self.inventory.get('thystame', 0)}"
            secret_message = self.comms.encode(ping_content, "ping")
            self.cmd.send_command(f"Broadcast {secret_message}")
        self.message_stack = self.analyze_calls()
        self.process_radio_and_demographics()
        return

    def analyze_calls(self): 
        """
        Analyse les messages radio non lus.
        Retourne : {id_agent : [{k : (command, [params])}], id_agent2 : ...}
        """
        parsed_calls = {}
        self.unread_messages = self.cmd.get_unread_broadcasts()
        self.decoded_messages = []
        for msg in self.unread_messages:
            try:
                clean_msg = msg.replace("message ", "")
                parts = clean_msg.split(",", 1)
                k = int(parts[0].strip())
                raw_text = parts[1].strip()
                decoded = self.comms.decode(raw_text)
                if decoded:
                    command = decoded.get("command")
                    raw_params = decoded.get("param", "") 
                    param_parts = raw_params.split('-')
                    sender_id = int(param_parts[0])
                    params_list = param_parts[1:]
                    decoded['param'] = params_list
                    self.decoded_messages.append({'k': k, 'data': decoded})
                    if sender_id not in parsed_calls:
                        parsed_calls[sender_id] = []
                    parsed_calls[sender_id].append({k: (command, params_list)})
            except Exception as e:
                continue
        return parsed_calls

    def process_radio_and_demographics(self):
        """Traite les messages radio reçus pour mettre à jour la liste des alliés connus,
        leur niveau, et calculer l'inventaire global des agents d'élévation (gardes + reine).\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        for agent_id, messages in self.message_stack.items():
            if agent_id not in self.team_roster:
                self.team_roster[agent_id] = {"last_seen": 0, "level": 1}
            self.team_roster[agent_id]["last_seen"] = 0
            for msg_dict in messages:
                for k, (command, params_list) in msg_dict.items():
                    if command == "ping" and len(params_list) >= 1:
                        try:
                            self.team_roster[agent_id]["level"] = int(params_list[0])
                        except (ValueError, IndexError):
                            pass
        for agent_id in list(self.team_roster.keys()):
            if agent_id not in self.message_stack:
                self.team_roster[agent_id]["last_seen"] += 1
                if self.team_roster[agent_id]["last_seen"] > 15:
                    del self.team_roster[agent_id]



    def evaluate_situation(self):
        """Évalue la situation actuelle de l'agent pour décider de son rôle et état.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        for agent_id, messages in self.message_stack.items():
            for msg_dict in messages:
                for k, (command, params_list) in msg_dict.items():
                    if command in ["evaqueen", "grouping"]:
                        if agent_id > self.agent_id or not self.is_queen:
                            self.is_queen_elected = True
                            if self.is_queen and agent_id > self.agent_id:
                                logger.info(f"Une autre Reine avec un ID plus grand ({agent_id}) détectée. Je redeviens paysan.")
                                self.is_queen = False
                                self.role = AgentRole.PEASANT
                        break
        if self.is_queen:
            return
        if self.level >= 3 and not self.is_queen_elected:
            eligible_ids = [self.agent_id]
            for a_id, data in self.team_roster.items():
                if data.get("level", 1) >= 3:
                    eligible_ids.append(a_id)
                    
            if self.agent_id == max(eligible_ids):
                self.role = AgentRole.QUEEN
                self.is_queen = True
                self.is_queen_elected = True
                self.agent_id = 9999
                queen_msg = f"{self.agent_id}-{self.level}-{self.inventory.get('food', 0)}-{self.inventory.get('linemate', 0)}-{self.inventory.get('deraumere', 0)}-{self.inventory.get('sibur', 0)}-{self.inventory.get('mendiane', 0)}-{self.inventory.get('phiras', 0)}-{self.inventory.get('thystame', 0)}"
                secret_message = self.comms.encode(queen_msg, "evaqueen")
                self.cmd.send_command(f"Broadcast {secret_message}")
                logger.info(f"Agent élu REINE ! ID mis à: {self.agent_id}")
                return
        new_role = self.utils.which_role()
        if new_role != self.role:
            logger.info(f"Changement de rôle : {self.role} → {new_role}")
            self.role = new_role
        if self.level < 3 and self.role == AgentRole.PEASANT:
            if self.state not in (AgentState.GROUPING, AgentState.ELEVATION):
                self.state = AgentState.BEGINNER

    def act_based_on_role(self):
        """Fais agir l'agent en fonction de son rôle actuel.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais fais agir l'agent selon son rôle (queen, guard, protector, peasant)."""

        if self.eggs_laid >= 1:
            nb_mature_egg = self.cmd.send_command('Connect_nbr')
            if isinstance(nb_mature_egg, int) and nb_mature_egg >= 1:
                self.utils.launch_clone()
                self.eggs_laid = max(0, self.eggs_laid - 1)
        match self.role:
            case AgentRole.QUEEN:
                self.task.act_queen()
            case AgentRole.PEASANT:
                self.task.act_peasant()

    def run(self):
        """Lance la boucle principale de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais maintient la boucle de vie de l'agent tant qu'il est en vie."""

        if self.client_num > 0:
            self.utils.launch_clone()
            self.client_num = 0
        while self.is_alive:
            self.update_internal_state()
            if not self.is_alive:
                break
            self.evaluate_situation()
            if not self.is_alive:
                break                
            self.act_based_on_role()
            if not self.is_alive:
                break
