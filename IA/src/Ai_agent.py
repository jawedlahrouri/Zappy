#!/usr/bin/python3

import random
import time
import sys
import subprocess
from enum import Enum
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms
from src.agent_submodules.agent_state import StateComportment, AgentState 
from src.command import ZappyCommand
from src.utils import utils
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentCore")

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand, team_name: str, map_size : str, client_num):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms) # Module de groupage contenant la logique de regroupement de notre agent.
        self.utils = utils(self.cmd, team_name) # Utilitaires divers, comme le lancement de clones.
        self.comportment = StateComportment(self) # Comportement de l'agent selon son état (survie, farming, grouping, etc).

        self.team_name = team_name # Le nom de l'équipe.
        self.unread_messages = [] # Messages reçus du serveur mais pas encore traités.
        self.agent_id = random.randint(1000, 9999) # Un ID unique pour cet agent, utilisé dans les communications cryptées.
        self.is_alive = True # Flag pour savoir si l'agent est encore en vie.
        self.state = AgentState.SURVIVAL # État initial de l'agent, il commencera en mode survie pour sécuriser sa position avant de faire autre chose.
        self.level = 1 # Niveau actuel de l'agent, commence à 1 et augmente avec les incantations réussies.
        self.inventory = {} # Dictionnaire pour stocker les ressources que l'agent a en sa possession (nourriture, pierres, etc).
        self.vision = [] # Ce que l'agent voit autour de lui, mis à jour à chaque cycle.
        self.food_minimum = 6 # Seuil de nourriture en dessous duquel l'agent passe en mode survie pour éviter de mourir de faim (ça serait bete).
        self.food_safe = 15 # Seuil de nourriture au-dessus duquel l'agent se sent en sécurité pour faire des actions plus risquées comme le farming ou le regroupement.
        self.team_roster = {} # Dictionnaire pour suivre les alliés connus et leur dernière activité.
        self.population_target = 10 # Nombre d'agents que nous voulons idéalement dans l'équipe, utilisé pour décider quand faire du forking.
        self.last_ping_time = time.time() # Timestamp du dernier ping envoyé pour vérifier la présence des alliés.
        self.x = 0 # position en X
        self.y = 0 # position en Y
        self.direction = 0 # direction dans laquelle le robot regarde
        self.map_width, self.map_height = map_size[0], map_size[1] # taille de la map, utilisée pour le pathfinding et la navigation.
        self.client_num = client_num # Le nombre de slots clients attribués à notre équipe, utilisé pour décider si on doit faire du forking dès le départ.

    def update_internal_state(self):
        """Met à jour la vision, l'inventaire, et les messages reçus du serveur.
        Vérifie aussi la connexion et la survie de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        if not self.cmd.network.connected:
            self.is_alive = False
            logger.error("Connexion au serveur perdue.")
            return
        response = self.cmd.send_command("Look")
        if response == "dead":
            self.is_alive = False
            logger.warning(f"L'agent {self.agent_id} est mort. Il etait niveau {self.level}. Agent restant: {len(self.team_roster)}.")
            return
        self.vision = response
        response = self.cmd.send_command("Inventory")
        if response == "dead":
            self.is_alive = False
            logger.warning(f"L'agent {self.agent_id} est mort. Il etait niveau {self.level}. Agent restant: {len(self.team_roster)}.")
            return
        self.inventory = response
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
                    self.decoded_messages.append({'k': k, 'data': decoded})
            except Exception:
                continue
        current_time = time.time()
        if current_time - self.last_ping_time > 15:
            secret_msg = self.comms.encode(str(self.agent_id), "ping")
            self.cmd.send_command(f"Broadcast {secret_msg}")
            self.last_ping_time = current_time

    def process_radio_and_demographics(self):
        """Traite les messages radio reçus pour mettre à jour la liste des alliés connus
        et détecter les agents qui ne répondent plus.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        current_time = time.time()
        for msg_obj in self.decoded_messages:
            decoded = msg_obj['data']
            if decoded['command'] == 'ping':
                sender_id = int(decoded['param'])
                self.team_roster[sender_id] = current_time
            elif decoded['command'] == 'needmate':
                sender_id = int(decoded['param'].split('-')[1])
                self.team_roster[sender_id] = current_time
        dead_ids = [ally_id for ally_id, last_seen in self.team_roster.items() if current_time - last_seen > 40]
        for ally_id in dead_ids:
            logger.warning(f"Allié {ally_id} ne répond plus. Déclaré mort.")
            del self.team_roster[ally_id]

    def evaluate_situation(self):
        """Évalue la situation actuelle de l'agent pour décider de son état (survie, farming, grouping, etc).\n
        Params:
            -Aucun paramètre.
        Retourne immédiatement si l'agent est déjà en mode ÉLÉVATION, sinon retourne l'état décidé après évaluation."""

        if self.state == AgentState.ELEVATION:
            return
        self.process_radio_and_demographics()
        food = self.inventory.get('food', 0)
        if food <= self.food_minimum:
            self.state = AgentState.SURVIVAL
            return
        call_k = self.grouper.analyze_calls(self.level, self.agent_id, self.decoded_messages)
        if call_k is not None:
            logger.info(f"l'agent {self.agent_id} a reçu un appel radio valide pour le regroupement (K={call_k}).")
            self.grouper.current_k = call_k
            self.state = AgentState.GROUPING
            return
        if self.farmer.can_elevate(self.level, self.inventory):
            return self.elevate_conditions(food)
        current_population = len(self.team_roster) + 1
        if current_population < self.population_target:
            if self.check_population(food, current_population) == 1: 
                return
        if food > self.food_safe:
            self.state = AgentState.FARMING
            return
        self.state = AgentState.SURVIVAL

    def check_population(self, food, current_population):
        """Vérifie si les conditions sont réunies pour pondre un oeuf\n
        Params:
            -food (int): La quantité de nourriture actuellement dans l'inventaire de l'agent.
            -current_population (int): Le nombre actuel d'agents connus dans l'équipe
        Retourne 1 si on décide de faire de pondre un oeuf, 0 sinon."""

        is_queen = True
        for ally_id in self.team_roster.keys():
            if ally_id > self.agent_id:
                is_queen = False
                break
        if is_queen and food > self.food_safe:
            if not self.utils.is_food_critical_for_action(42, self.inventory, self.food_minimum):
                logger.info(f"l'agent {self.agent_id} est la REINE (ID {self.agent_id}). Pop actuelle: {current_population}/{self.population_target}.")
                self.state = AgentState.FORKING
                return 1
        return 0

    def elevate_conditions(self, food):
        """Vérifie si les conditions sont réunies pour tenter une incantation\n
        Params:
            -food (int): La quantité de nourriture actuellement dans l'inventaire de l'agent.
        Retourne True si on décide de tenter une incantation, False sinon."""

        if food < (self.food_minimum + 3):
            logger.warning(f"l'agent {self.agent_id} n'a pas assez de nourriture pour tenter une incantation en toute sécurité (food={food}).")
            self.state = AgentState.SURVIVAL
            return
        logger.info(f"l'agent {self.agent_id} a toutes les ressources nécessaires pour l'incantation au Niveau {self.level}.")
        if self.level == 1:
            self.state = AgentState.ELEVATION
        else:
            self.state = AgentState.GROUPING
        return

    def act_based_on_state(self):
        """"Fais agir l'agent en fonction de son état actuel.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais fais agir l'agent selon son état (survie, farming, grouping, etc)."""

        if self.state == AgentState.SURVIVAL:
            self.state = self.comportment.act_survival()
        elif self.state == AgentState.FARMING:
            self.state = self.comportment.act_farming()
        elif self.state == AgentState.GROUPING:
            self.state = self.comportment.act_grouping()
        elif self.state == AgentState.ELEVATION:
            self.state = self.comportment.act_elevating()
        elif self.state == AgentState.FORKING:
            self.state = self.comportment.act_forking()

    def run(self):
        """Lance la boucle principale de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais maintient la boucle de vie de l'agent tant qu'il est en vie."""

        logger.info(f"l'agent {self.agent_id} démarre le cycle de vie.")
        if self.client_num >= 0:
            self.utils.launch_clone() 
        while self.is_alive:
            self.update_internal_state()
            if not self.is_alive:
                break
            self.evaluate_situation()
            if not self.is_alive:
                break                
            self.act_based_on_state()
            time.sleep(0.1)
        self.cmd.network.close()
        logger.info(f"l'agent {self.agent_id} a terminé son cycle de vie (Déconnecté).")
