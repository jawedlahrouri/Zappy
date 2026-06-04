#!/usr/bin/python3

import random
import math
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
        self.food_minimum = 5
        self.food_safe = 15

    def update_internal_state(self):
        self.inventory = self.cmd.inventory()
        self.vision = self.cmd.look()

    def evaluate_situation(self):
        food = self.inventory.get('food', 0)

        if food <= self.food_minimum:
            self.state = AgentState.SURVIVAL
            return
        if self.farmer.can_elevate(self.level, self.inventory): 
            if self.level == 1:
                self.state = AgentState.ELEVATION
            else:
                self.state = AgentState.GROUPING
            return
        if food > self.food_safe:
            self.state = AgentState.FARMING

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
                print("[Agent]  exploration par défaut.")
                self.pathfinder.explore(self.level)
        elif self.state == AgentState.GROUPING:
            is_grouped = self.grouper.listen_and_group(self.level)
            if is_grouped:
                print("[Agent] Regroupement réussi ! Passage en phase d'Incantation.")
                self.state = AgentState.ELEVATION
        elif self.state == AgentState.ELEVATION:
            print(f"[Agent] Mode ÉLÉVATION (Niveau {self.level}) : préparation du rituel.")
            if self.level == 1 or self.grouper.role == 'LEADER':
                required_stones = self.farmer.requirements.get(self.level, {})
                for stone_name, amount in required_stones.items():
                    for _ in range(amount):
                        self.cmd.set_object(stone_name)
                print("[Agent] Pierres posées. Lancement de l'Incantation !")
                success = self.cmd.incantation()
                if success:
                    self.level += 1
                    print(f"[Agent] SUCCÈS! je passe au Niveau {self.level} !")
                    self.grouper.role = None
                else:
                    print("[Agent] ❌ Échec de l'incantation.")
            elif self.grouper.role == 'FOLLOWER':
                success = self.cmd.wait_for_incantation_follower()
                if success:
                    self.level += 1
                    print(f"[Agent] SUCCÈS ! Le FOLLOWER passe au Niveau {self.level} !")
                    self.grouper.role = None  # On reset
                else:
                    print("[Agent] ❌ Échec de l'incantation du Leader.")
                    self.grouper.role = None

    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            if not self.inventory:
                self.is_alive = False
                break
            self.evaluate_situation()
            self.act_based_on_state()

