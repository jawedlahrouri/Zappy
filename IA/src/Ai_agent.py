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

        self.unread_messages = []
        self.agent_id = random.randint(1000, 9999)
        self.is_alive = True
        self.state = AgentState.SURVIVAL
        self.level = 1
        self.inventory = {}
        self.vision = []
        self.food_minimum = 5
        self.food_safe = 15

    #mets à jour l'état interne de l'agent et vérifie s'il est encore en vie
    def update_internal_state(self):
        if not self.cmd.network.connected:
            self.is_alive = False
            print("[Agent] Connexion perdue.")
            return
        
        response = self.cmd.look()
        if response == "dead":
            self.is_alive = False
            return
        self.vision = response
        
        response = self.cmd.inventory()
        if response == "dead":
            self.is_alive = False
            return
        self.inventory = response
        self.unread_messages = self.cmd.get_unread_broadcasts()

    def evaluate_situation(self):
        if self.state == AgentState.ELEVATION:
            return
        food = self.inventory.get('food', 0)
        if food <= self.food_minimum:
            self.state = AgentState.SURVIVAL
            return
        call_k = self.grouper.analyze_calls(self.level, self.agent_id, self.unread_messages)
        
        if call_k is not None:
            print(f"[Agent] Appel radio valide reçu (K={call_k}) ! On passe en GROUPING.")
            self.grouper.current_k = call_k
            self.state = AgentState.GROUPING
            return
        if self.farmer.can_elevate(self.level, self.inventory):
            if food < (self.food_minimum + 3):
                print("[Agent] Pierres OK, mais nourriture insuffisante pour le rituel. Survie !")
                self.state = AgentState.SURVIVAL
                return
            print(f"[Agent] Ressources d'élévation prêtes pour le Niveau {self.level} ! Passage en mode ÉLÉVATION.")
            if self.level == 1:
                self.state = AgentState.ELEVATION
            else:
                self.state = AgentState.GROUPING
            return
        if food > self.food_safe:
            self.state = AgentState.FARMING
            return
        self.state = AgentState.SURVIVAL

    def act_based_on_state(self):
        if self.state == AgentState.SURVIVAL:
            success = self.pathfinder.search_and_take_items(['food'], self.vision)
            if not success:
                self.pathfinder.explore(self.level)
        elif self.state == AgentState.FARMING:
            missing = self.farmer.get_missing_stones(self.level, self.inventory)
            if missing:
                success = self.pathfinder.search_and_take_items(missing, self.vision)
                if not success:
                    self.pathfinder.explore(self.level)
            else:
                self.pathfinder.explore(self.level)

        elif self.state == AgentState.GROUPING:
            is_grouped = self.grouper.process_grouping(self.level, self.agent_id, self.vision)            
            if is_grouped:
                print("[Agent] Regroupement validé ! Passage en phase d'Incantation.")
                self.state = AgentState.ELEVATION

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
                    print(f"[Agent] SUCCÈS, passe au Niveau {self.level} !")
                else:
                    print("[Agent] Échec de l'incantation.")
                
            elif self.grouper.role == 'FOLLOWER':
                success = self.cmd.wait_for_incantation_follower()
                if success:
                    self.level += 1
                    print(f"[Agent] FOLLOWER passe au Niveau {self.level} !")
            self.grouper.role = None
            self.state = AgentState.SURVIVAL
            #TODO: les gars ajoutez ici les futurs comportement je ferais une map quand j'aurais moins la flemme

    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            self.evaluate_situation()
            self.act_based_on_state()
            time.sleep(0.2)

    def is_food_critical_for_action(self, estimated_tu_cost):
        food = self.inventory.get('food', 0)
        current_tu = food * 126
        minimum_tu_required = (self.food_minimum * 126)
        if current_tu < (estimated_tu_cost + minimum_tu_required):
            return True
        return False
