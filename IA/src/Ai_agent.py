#!/usr/bin/python3

import random
import math
import time
from enum import Enum
from src.command import ZappyCommand
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms

class AgentState(Enum):
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand):
        self.cmd = command_interface
        self.pathfinder = Pathfinder(self.cmd)
        self.farmer = Farmer()
        self.comms = StealthComms()
        self.grouper = Grouper(self.cmd, self.comms)

        self.is_alive = True
        self.state = AgentState.SURVIVAL
        self.level = 1
        self.inventory = {}
        self.vision = []
        self.food_minimum = 50
        self.food_safe = 100

    #mets à jour l'état interne de l'agent et vérifie s'il est encore en vie
    def update_internal_state(self):
        if not self.cmd.network.connected:
            self.is_alive = False
            print("[Agent] Connecté perdue.")
            return
        
        response = self.cmd.look()
        if response == "dead":
            self.is_alive = False
            print("[Agent] Je suis mort.")
            return
        self.vision = response
        
        response = self.cmd.inventory()
        if response == "dead":
            self.is_alive = False
            print("[Agent] Je suis mort.")
            return
        self.inventory = response

        food = self.inventory.get('food', 0)
        if food <= 0:
            print("[Agent] Plus de nourriture, c'est la mort.")
            self.is_alive = False

    # verifie la situation actuelle et décide du mode de l'agent
    def evaluate_situation(self):
        food = self.inventory.get('food', 0)
        if food <= self.food_minimum:
            print(f"[Agent] SURVIVAL MODE: nourriture {food} <= limite {self.food_minimum}")
            self.state = AgentState.SURVIVAL
            return

        if self.farmer.can_elevate(self.level, self.inventory): 
            print(f"[Agent] Ressources d'élévation détectées!")
            if self.level == 1:
                self.state = AgentState.ELEVATION
            else:
                self.state = AgentState.GROUPING
            return

        if food > self.food_safe:
            print(f"[Agent] FARMING MODE: nourriture {food}")
            self.state = AgentState.FARMING
            return
        
        if self.state != AgentState.FARMING:
            print(f"[Agent] MODE par défaut: SURVIVAL (attente)")
            self.state = AgentState.SURVIVAL

    def act_based_on_state(self):
        #methode survival : cherche de la nourriture en priorité sinon explore
        if self.state == AgentState.SURVIVAL:
            success = self.pathfinder.search_and_take_items(['food'], self.vision)
            if not success:
                self.pathfinder.explore(self.level)

        #methode farming : cherche les ressources d'élévation manquantes sinon explore
        elif self.state == AgentState.FARMING:
            missing = self.farmer.get_missing_stones(self.level, self.inventory)
            if missing:
                success = self.pathfinder.search_and_take_items(missing, self.vision)
                if not success:
                    self.pathfinder.explore(self.level)
            else:
                print("[Agent]  exploration par défaut.")
                self.pathfinder.explore(self.level)

        #methode grouping : écoute les autres agents pour se regrouper si possible, sinon explore
        elif self.state == AgentState.GROUPING:
            is_grouped = self.grouper.listen_and_group(self.level)
            if is_grouped:
                print("[Agent] Regroupement réussi ! Passage en phase d'Incantation.")
                self.state = AgentState.ELEVATION
        
        #methode elevation : si leader, prépare le rituel, sinon attend le rituel du leader
        elif self.state == AgentState.ELEVATION:
            print(f"[Agent] Mode ÉLÉVATION (Niveau {self.level}) : préparation du rituel.")
            if self.level == 1 or self.grouper.role == 'LEADER':
                required_stones = self.farmer.requirements.get(self.level, {})
                for stone_name, amount in required_stones.items():
                    for _ in range(amount):
                        self.cmd.set_object(stone_name)
                success = self.cmd.incantation()
                if success:
                    self.level += 1
                    print(f"[Agent] SUCCÈS! je passe au Niveau {self.level} !")
                    self.grouper.role = None
                else:
                    print("[Agent] Échec de l'incantation.")
            elif self.grouper.role == 'FOLLOWER':
                success = self.cmd.wait_for_incantation_follower()
                if success:
                    self.level += 1
                    print(f"[Agent] FOLLOWER passe au Niveau {self.level}")
                    self.grouper.role = None
            #TODo: les gars ajoutez ici les futurs comportement je ferais une map quand j'aurais moins la flemme

    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            self.evaluate_situation()
            self.act_based_on_state()
            time.sleep(0.2)
