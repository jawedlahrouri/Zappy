#!/usr/bin/python3

import decimal
import random
import sys
import subprocess
from enum import Enum
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

# RIP balise we loved you, you'll always be tung tung sahere

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand, team_name: str, map_size : str, client_num):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.cmd.on_success_callback = self.handle_command_success
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms, self) # Module de groupage contenant la logique de regroupement de notre agent.
        self.utils = utils(self.cmd, team_name) # Utilitaires divers, comme le lancement de clones.
        self.comportment = StateComportment(self) # Comportement de l'agent selon son état (survie, farming, grouping, etc).
        self.task = RoleTask(self) # Role de l'agent

        self.client_num = client_num # Le nombre de slots clients attribués à notre équipe, utilisé pour décider si on doit faire du forking dès le départ.
        self.team_name = team_name # Le nom de l'équipe.
        self.unread_messages = [] # Messages reçus du serveur mais pas encore traités.
        self.agent_id = random.randint(1000, 9998) # Un ID unique pour cet agent, utilisé dans les communications cryptées.
        self.is_alive = True # Flag pour savoir si l'agent est encore en vie.
        self.state = AgentState.BEGINNER # État initial de l'agent, il commencera en mode survie pour sécuriser sa position avant de faire autre chose.
        self.role = AgentRole.PEASANT
        self.level = 1 # Niveau actuel de l'agent, commence à 1 et augmente avec les incantations réussies.
        self.inventory = {} # Dictionnaire pour stocker les ressources que l'agent a en sa possession (nourriture, pierres, etc).
        self.vision = [] # Ce que l'agent voit autour de lui, mis à jour à chaque cycle.
        self.food_minimum = 6 # Seuil de nourriture en dessous duquel l'agent passe en mode survie pour éviter de mourir de faim (ça serait bete).
        self.food_safe = 15 # Seuil de nourriture au-dessus duquel l'agent se sent en sécurité pour faire des actions plus risquées comme le farming ou le regroupement.
        self.team_roster = {} # Dictionnaire pour suivre les alliés connus et leur dernière activité.
        self.population_target = 10 # Nombre d'agents que nous voulons idéalement dans l'équipe, utilisé pour décider quand faire du forking.
        self.is_queen_elected = False # Flag pour savoir si la reine a été élue.
        self.is_queen = False # Flag pour savoir si cet agent est la reine.
        self.eggs_laid = 0 # Nombre d'oeufs pondus par la reine.
        self.decret_royal_append = False # est ce que les guards et protector ont etait élu par la reine
        self.decret_royal = "" #decret royal envoyé par la reine
        self.message_stack = {}
        self.global_inventory = {} # Dictionnaire pour suivre les ressources globales de l'équipe, mis à jour via les messages radio.
        self.direction = 0 # direction dans laquelle le robot regarde
        self.k_queen = 0

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
        self.message_stack = self.analyze_calls()
        self.process_radio_and_demographics()
        ping_content = f"{self.agent_id}-{self.level}-{self.inventory.get('food', 0)}-{self.inventory.get('linemate', 0)}-{self.inventory.get('deraumere', 0)}-{self.inventory.get('sibur', 0)}-{self.inventory.get('mendiane', 0)}-{self.inventory.get('phiras', 0)}-{self.inventory.get('thystame', 0)}"
        secret_message = self.comms.encode(ping_content, "ping")
        self.cmd.send_command(f"broadcast {secret_message}")
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
        """Traite les messages radio reçus pour mettre à jour la liste des alliés connus
        et détecter les agents qui ne répondent plus.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        for agent_id, messages in self.message_stack.items():
            if agent_id not in self.team_roster:
                self.team_roster[agent_id] = {"last_seen": 0, "level": 1}
            self.team_roster[agent_id]["last_seen"] = 0
        for agent_id in list(self.team_roster.keys()):
            if agent_id not in self.message_stack:
                self.team_roster[agent_id]["last_seen"] += 1
                if self.team_roster[agent_id]["last_seen"] > 15: #TODO: check avg cycle duration and adapt
                    del self.team_roster[agent_id]

    def evaluate_situation(self):
        """Évalue la situation actuelle de l'agent pour décider de son état (survie, farming, grouping, etc).\n
        Params:
            -Aucun paramètre.
        Retourne immédiatement si l'agent est déjà en mode ÉLÉVATION, sinon retourne l'état décidé après évaluation."""

        if self.level < 2 and self.role == AgentRole.PEASANT:
            self.state = AgentState.BEGINNER
        if self.level >= 2 and not self.is_queen_elected and self.agent_id == max(self.team_roster): # peut etre sujet a des problemes
            self.state = AgentState.SURVIVAL
            self.role = AgentRole.QUEEN
            self.is_queen == True
            self.agent_id = 9999
        self.role = AgentRole.self.utils.which_role()

    def check_population(self, food, current_population):
        """Vérifie si les conditions sont réunies pour pondre un oeuf\n
        Params:
            -food (int): La quantité de nourriture actuellement dans l'inventaire de l'agent.
            -current_population (int): Le nombre actuel d'agents connus dans l'équipe
        Retourne 1 si on décide de faire de pondre un oeuf, 0 sinon."""

    def elevate_conditions(self, food):
        """Vérifie si les conditions sont réunies pour tenter une incantation\n
        Params:
            -food (int): La quantité de nourriture actuellement dans l'inventaire de l'agent.
        Retourne True si on décide de tenter une incantation, False sinon."""

    def act_based_on_role(self):
        """"Fais agir l'agent en fonction de son état actuel.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais fais agir l'agent selon son état (survie, farming, grouping, etc)."""

        match self.state:
            case AgentRole.QUEEN:
                self.role = self.task.act_queen()
            case AgentRole.GUARD:
                self.role = self.task.act_guard()
            case AgentRole.PROTECTOR:
                self.role = self.task.act_protector()
            case AgentRole.PEASANT:
                self.role = self.task.act_peasant()

    def act_based_on_state(self):
        """"Fais agir l'agent en fonction de son état actuel.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais fais agir l'agent selon son état (survie, farming, grouping, etc)."""

        match self.state:
            case AgentState.SURVIVAL:
                self.state = self.comportment.act_survival()
            case AgentState.FARMING:
                self.state = self.comportment.act_farming()
            case AgentState.GROUPING:
                self.state = self.comportment.act_grouping()
            case AgentState.ELEVATION:
                self.state = self.comportment.act_elevating()
            case AgentState.BEGINNER:
                self.state = self.comportment.act_beginer()
            case AgentState.FEEDER:
                self.state = self.comportment.act_feeder()
            case AgentState.PUSHING:
                self.state = self.comportment.act_pushing()
        
    def run(self):
        """Lance la boucle principale de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais maintient la boucle de vie de l'agent tant qu'il est en vie."""

        if self.client_num > 0:
            self.utils.launch_clone()
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
