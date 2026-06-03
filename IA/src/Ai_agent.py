#!/usr/bin/python3

import random
import math
from enum import Enum
from src.command import ZappyCommand
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer

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
    
    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            if not self.inventory:
                self.is_alive = False
                break
            self.evaluate_situation()
            self.act_based_on_state()

