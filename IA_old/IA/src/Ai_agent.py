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
from src.command import ZappyCommand
from src.utils import utils
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentCore")

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand, team_name: str, map_size : str, client_num):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.cmd.on_success_callback = self.handle_command_success
        self.pathfinder = Pathfinder(self.cmd) # Module de pathfinding pour se déplacer efficacement.
        self.farmer = Farmer() # Module de gestion des ressources pour savoir quoi collecter.
        self.comms = StealthComms() # Module de cryptage des message pour sécuriser la communication entre nos agents.
        self.grouper = Grouper(self.cmd, self.comms) # Module de groupage contenant la logique de regroupement de notre agent.
        self.utils = utils(self.cmd, team_name) # Utilitaires divers, comme le lancement de clones.
        self.comportment = StateComportment(self) # Comportement de l'agent selon son état (survie, farming, grouping, etc).
        
        self.client_num = client_num # Le nombre de slots clients attribués à notre équipe, utilisé pour décider si on doit faire du forking dès le départ.
        self.team_name = team_name # Le nom de l'équipe.
        self.unread_messages = [] # Messages reçus du serveur mais pas encore traités.
        self.agent_id = random.randint(1000, 9999) # Un ID unique pour cet agent, utilisé dans les communications cryptées.
        self.is_alive = True # Flag pour savoir si l'agent est encore en vie.
        self.state = AgentState.BEGINNER # État initial de l'agent, il commencera en mode survie pour sécuriser sa position avant de faire autre chose.
        self.level = 1 # Niveau actuel de l'agent, commence à 1 et augmente avec les incantations réussies.
        self.inventory = {} # Dictionnaire pour stocker les ressources que l'agent a en sa possession (nourriture, pierres, etc).
        self.vision = [] # Ce que l'agent voit autour de lui, mis à jour à chaque cycle.
        self.food_minimum = 6 # Seuil de nourriture en dessous duquel l'agent passe en mode survie pour éviter de mourir de faim (ça serait bete).
        self.food_safe = 15 # Seuil de nourriture au-dessus duquel l'agent se sent en sécurité pour faire des actions plus risquées comme le farming ou le regroupement.
        self.team_roster = {} # Dictionnaire pour suivre les alliés connus et leur dernière activité.
        self.population_target = 10 # Nombre d'agents que nous voulons idéalement dans l'équipe, utilisé pour décider quand faire du forking.
        self.last_ping_cycle = 0 # Cycle de vie lors du dernier ping envoyé pour vérifier la présence des alliés.
        self.x = 0 # position en X
        self.y = 0 # position en Y
        self.direction = 0 # direction dans laquelle le robot regarde
        if isinstance(map_size, tuple):
            self.map_width, self.map_height = map_size # gestion de si les données de la map sont obtenues sous forme de tuple
        else:
            self.map_width, self.map_height = map_size.split() # gestion de si les données de la map sont obtenues sous forme de string
        self.map_width = int(self.map_width) # largeur de la map
        self.map_height = int(self.map_height) # hauteur de la map
        self.ping_sent = False # ping de naissance envoyé ou non
        self.wait_counter = 0 # temps d'attente avant de commencer a envoyer des messages
        self.balise_k = None # direction de la balise (set ses coords a zero)
        self.heard_balise = False # Indique si l'agent a déjà entendu la balise
        self.cycles_lived = 0 # Compte le nombre de cycles de vie de l'agent.
        self.is_queen_elected = False # Flag pour savoir si la reine a été élue.
        self.is_queen = False # Flag pour savoir si cet agent est la reine.
        self.last_connect_nbr = client_num # Nombre de slots clients lors du dernier check, utilisé pour détecter les oeufs éclos.
        self.unclaimed_clones = 0 # Nombre d'agents nés mais pas encore connectés.
        self.eggs_laid = 0 # Nombre d'oeufs pondus par la reine.
        self.initial_cascade_pending = True # Flag pour savoir si la cascade initiale de forking est en cours.

    def handle_command_success(self, cmd_name: str):
        """Met à jour la position et direction internes suite à une commande réussie."""
        if cmd_name == "Forward":
            if self.direction == 0:
                self.y += 1
            elif self.direction == 1:
                self.x += 1
            elif self.direction == 2:
                self.y -= 1
            elif self.direction == 3:
                self.x -= 1
            
            self.x = self.x % self.map_width
            self.y = self.y % self.map_height
            logger.info(f"Agent {self.agent_id} position mise à jour: x={self.x}, y={self.y}")

        elif cmd_name == "Right":
            self.direction = (self.direction + 1) % 4
            logger.info(f"Agent {self.agent_id} direction mise à jour (Right): dir={self.direction}")

        elif cmd_name == "Left":
            self.direction = (self.direction - 1) % 4
            logger.info(f"Agent {self.agent_id} direction mise à jour (Left): dir={self.direction}")


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
        if self.cycles_lived - self.last_ping_cycle > 10:
            secret_msg = self.comms.encode(str(self.agent_id), "ping")
            self.cmd.send_command(f"Broadcast {secret_msg}")
            self.last_ping_cycle = self.cycles_lived

    def process_radio_and_demographics(self):
        """Traite les messages radio reçus pour mettre à jour la liste des alliés connus
        et détecter les agents qui ne répondent plus.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien."""

        for msg_obj in self.decoded_messages:
            decoded = msg_obj['data']
            if decoded['command'] == 'ping':
                sender_id = int(decoded['param'])
                self.team_roster[sender_id] = self.cycles_lived
            elif decoded['command'] == 'needmate':
                sender_id = int(decoded['param'].split('-')[1])
                self.team_roster[sender_id] = current_time
            elif decoded['command'] == 'balise':
                self.balise_k = msg_obj['k']
                self.heard_balise = True

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
        self.cycles_lived += 1
        if not self.is_queen_elected and self.cycles_lived > 5:
            self.elect_queen()  
        food = self.inventory.get('food', 0)
        
        if self.state not in [AgentState.BEGINNER, AgentState.BALISE, AgentState.FEEDER]:
            if food > 25 and self.heard_balise:
                logger.info(f"L'agent {self.agent_id} a beaucoup de nourriture ({food}). Il passe en mode FEEDER pour ravitailler la balise.")
                self.state = AgentState.FEEDER
                return

        if food <= self.food_minimum and self.state != AgentState.BALISE:
            self.state = AgentState.SURVIVAL
            return
        if self.state == AgentState.BEGINNER and self.balise_k is not None:
            if self.balise_k == 0:
                self.x = 0
                self.y = 0
                self.state = AgentState.SURVIVAL
                self.balise_k = None
                logger.info(f"L'agent {self.agent_id} est sur la balise. Coordonnées calibrées à (0,0). Passage en mode SURVIVAL.")
                return
            else:
                return
        if self.state in [AgentState.BEGINNER, AgentState.BALISE]:
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

    def elect_queen(self):
        """Détermine si cet agent doit devenir la Reine, après avoir laissé le temps aux pings d'arriver.
        Params:
            -Aucun paramètre.
        Retourne rien, mais défini l'agent comme reine si les conditions sont réunies.
        """

        highest_id = self.agent_id
        for ally_id in self.team_roster.keys():
            if ally_id > highest_id:
                highest_id = ally_id
        if highest_id == self.agent_id:
            self.is_queen = True
            self.agent_id = 9999
            logger.info("Agent élu comme REINE (ID 9999).")
        self.is_queen_elected = True

    def check_population(self, food, current_population):
        """Vérifie si les conditions sont réunies pour pondre un oeuf\n
        Params:
            -food (int): La quantité de nourriture actuellement dans l'inventaire de l'agent.
            -current_population (int): Le nombre actuel d'agents connus dans l'équipe
        Retourne 1 si on décide de faire de pondre un oeuf, 0 sinon."""

        if self.is_queen and food > self.food_safe:
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

        match self.state:
            case AgentState.SURVIVAL:
                self.state = self.comportment.act_survival()
            case AgentState.FARMING:
                self.state = self.comportment.act_farming()
            case AgentState.GROUPING:
                self.state = self.comportment.act_grouping()
            case AgentState.ELEVATION:
                self.state = self.comportment.act_elevating()
            case AgentState.FORKING:
                self.state = self.comportment.act_forking()
            case AgentState.BALISE:
                self.state = self.comportment.act_balise()
            case AgentState.BEGINNER:
                self.state = self.comportment.act_beginer()
            case AgentState.FEEDER:
                self.state = self.comportment.act_feeder()
        
    def run(self):
        """Lance la boucle principale de l'agent.\n
        Params:
            -Aucun paramètre.
        Ne retourne rien, mais maintient la boucle de vie de l'agent tant qu'il est en vie."""

        logger.info(f"l'agent {self.agent_id} démarre le cycle de vie.")
        while self.is_alive:
            self.update_internal_state()
            if not self.is_alive:
                break
            self.evaluate_situation()
            if not self.is_alive:
                break                
            self.act_based_on_state()
            if self.is_queen_elected:
                self.manage_demographics()
        self.cmd.network.close()
        logger.info(f"l'agent {self.agent_id} a terminé son cycle de vie (Déconnecté).")
