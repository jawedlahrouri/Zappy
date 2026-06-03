#!/usr/bin/python3

import random
import math
from enum import Enum
from src.command import ZappyCommand

class AgentState(Enum):
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand):
        self.cmd = command_interface
        self.is_alive = True
        self.state = AgentState.SURVIVAL
        self.level = 1
        self.inventory = {}
        self.vision = []
        self.food_safe = 15
        self.food_minimum = 5
        self.explore_step = 0

    def update_internal_state(self):
        self.inventory = self.cmd.inventory()
        self.vision = self.cmd.look()

    def evaluate_situation(self):
        food = self.inventory.get('food', 0)

        if food <= self.food_minimum:
            self.state = AgentState.SURVIVAL
            return
        if self.can_elevate():
            if self.level == 1:
                self.state = AgentState.ELEVATION
            else:
                self.state = AgentState.GROUPING
            return
        if food > self.food_safe:
            self.state = AgentState.FARMING

    def get_missing_stones(self):
        requirements = {
            1: {'linemate': 1},
            2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
            4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
            5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
            7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
        }
        needed = requirements.get(self.level, {})
        missing = []
        for item, amount in needed.items():
            if self.inventory.get(item, 0) < amount:
                missing.append(item)                
        return missing

    def can_elevate(self):
        requirements = {
            1: {'linemate': 1},
            2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
            4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
            5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
            7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
        }

        needed = requirements.get(self.level)
        if needed is None:
            return False
        return all(self.inventory.get(item, 0) >= amount for item, amount in needed.items())

    def act_based_on_state(self):
        if self.state == AgentState.SURVIVAL:
            print("[Agent] Mode SURVIE: recherche de nourriture.")
            self.search_and_take_items(['food'])
        elif self.state == AgentState.FARMING:
            print("[Agent] Mode FARMING: recherche de pierres.")
            missing_stones = self.get_missing_stones()
            if missing_stones:
                self.search_and_take_items(missing_stones)
        elif self.state == AgentState.ELEVATION:
            print("[Agent] Mode ÉLÉVATION : lancement de l'incantation !")
            self.cmd.set_object('linemate')
            success = self.cmd.incantation()
            if success:
                self.level += 1
                print(f"[Agent] Succès ! Passage au niveau {self.level}")

    def explore(self):
        if self.level <= 2:
            if self.explore_step < 3:
                print(f"[Agent Lvl {self.level}] Exploration : On trace tout droit.")
                self.cmd.forward()
                self.explore_step += 1
            else:
                print(f"[Agent Lvl {self.level}] Exploration : Virage pour quadriller la zone.")
                action = random.choice([self.cmd.right, self.cmd.left])
                action()
                self.explore_step = 0 
        else:
            if self.explore_step < 3:
                print(f"[Agent Lvl {self.level}] Exploration : Scan rotation droite ({self.explore_step + 1}/3)")
                self.cmd.right()
                self.explore_step += 1
            else:
                print(f"[Agent Lvl {self.level}] Exploration : Scan 360° terminé. On avance !")
                self.cmd.forward()
                self.explore_step = 0

    def search_and_take_items(self, target_items):
        best_index = -1
        best_score = -999
        for index, tile_contents in enumerate(self.vision):
            score = self._score_tile(index, tile_contents, target_items)
            if score > best_score:
                best_score = score
                best_index = index
        if best_index != -1:
            print(f"[Agent] Cible optimale: case {best_index} (Score: {best_score}). Déplacement...")
            self.explore_step = 0
            self.go_to_tile(best_index)
            if 'player' in self.vision[best_index] and best_index != 0:
                print("[Agent] Présence détectée sur la cible. EJECT !")
                self.cmd.eject()
            items_on_tile = self.vision[best_index]
            for item in target_items:
                count = items_on_tile.count(item)
                for _ in range(count):
                    print(f"[Agent] Ramassage multiple : {item}")
                    self.cmd.take_object(item)

            return True
        self.explore()
        return False


    def go_to_tile(self, target_index):
        if target_index == 0:
            return
        row = int(math.sqrt(target_index))
        for _ in range(row):
            self.cmd.forward()
        middle_of_row = row * row + row
        shift = target_index - middle_of_row
        if shift < 0:
            self.cmd.left()
            for _ in range(abs(shift)):
                self.cmd.forward()
        elif shift > 0:
            self.cmd.right()
            for _ in range(shift):
                self.cmd.forward()

    def _score_tile(self, index, tile_contents, target_items):
        score = 0
        items_found = 0
        for item in target_items:
            count = tile_contents.count(item)
            items_found += count
            score += count * 10 
        if items_found == 0:
            return -999
        row = int(math.sqrt(index))
        middle_of_row = row * row + row
        forward_cost = row 
        turn_cost = 1 if index != 0 and index != middle_of_row else 0
        
        distance_cost = forward_cost + turn_cost
        
        # On retire 2 points par action de mouvement nécessaire
        score -= distance_cost * 2 
        
        return score
    
    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            if not self.inventory:
                self.is_alive = False
                break
            self.evaluate_situation()
            self.act_based_on_state()

