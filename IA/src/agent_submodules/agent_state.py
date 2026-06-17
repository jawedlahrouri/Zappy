#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_role import AgentRole
from src.agent_submodules.pathfinding import Pathfinder
from src.command import ZappyCommand
from src.agent_submodules.farm import Farmer
from src.utils import utils
logger = ZappyLogger.get_logger("AgentState")

class AgentState(Enum): # Enum pour représenter les différents états de comportement de l'agent.
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4
    BEGINNER = 5
    LEADER = 6
    FOLLOWER = 7
    FEEDER = 8
    PUSHING = 9

class StateComportment:
    def __init__(self, agent):
        self.agent = agent # Référence à l'agent principal pour accéder à ses modules et données.
        self.pathfinding = Pathfinder
        self.command = ZappyCommand
        self.farm = Farmer
        self.utils = utils

    def act_survival(self):
        """Effectue les actions de survie de base, comme chercher de la nourriture pour éviter la mort par faim.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour survivre."""

        success = self.agent.pathfinder.search_and_take_items(['food'], self.agent.vision)
        if not success:
            self.agent.pathfinder.explore(self.agent.level)
        return AgentState.SURVIVAL

    def act_farming(self):
        """Effectue les actions de farming pour collecter les ressources nécessaires à l'incantation.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour farming."""
        
        if self.agent.role == self.agent.AgentRole.PEASANT or self.agent.role == self.agent.AgentRole.GUARD:
            success = self.agent.pathfinder.search_and_take_items("food", self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        if self.agent.role == self.agent.AgentRole.PROTECTOR or self.agent.role == self.agent.AgentRole.QUEEN:
            success = self.agent.pathfinder.search_and_take_items(self.agent.farm.get_missing_stones_for_max_level(self.agent.level, self.agent.global_inventory), self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        return AgentState.FARMING

    def act_elevating(self):
        """Effectue les actions nécessaires pour tenter une incantation, en fonction du rôle de l'agent dans le regroupement (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour l'incantation."""

        if self.agent.level == 1 or self.agent.grouper.role == 'LEADER' or self.agent.grouper.role == 'FOLLOWER':
            if self.agent.level == 1 or self.agent.grouper.role == 'LEADER':
                required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                for stone_name, amount in required_stones.items():
                    for _ in range(amount):
                        self.agent.cmd.send_command(f"Set {stone_name}")               
                success = self.agent.cmd.send_command("Incantation")
                if success:
                    self.agent.level += 1
            elif self.agent.grouper.role == 'FOLLOWER':
                success = self.agent.cmd.wait_for_incantation_follower()
                if success:
                    self.agent.level += 1
            self.agent.grouper.role = None
        elif self.agent.role == AgentRole.QUEEN or self.agent.role == AgentRole.GUARD:
            for stone_name, amount in self.agent.inventory.items():
                for _ in range(amount):
                    self.agent.cmd.send_command(f"Set {stone_name}")
            if self.agent.role == AgentRole.QUEEN:
                success = self.agent.cmd.send_command("Incantation")
                if success:
                    self.agent.level += 1
            elif self.agent.role == AgentRole.GUARD:
                success = self.agent.cmd.wait_for_incantation_follower()
                if success:
                    self.agent.level += 1
            self.act_elevating()
        return AgentState.SURVIVAL

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

        self.agent.grouper.process_grouping(self.agent.level, self.agent.id, self.agent.vision)

    def act_beginner(self):
        """Effectue les actions attendus de la part d'un bot qui vient d'arriver.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """
        if self.agent.level >= 3:
            self.agent.state = AgentState.FARMING
            return
        
        necessary_ressources = self.farm.get_missing_stones(self.agent.level, self.agent.inventory)
        if len(necessary_ressources) == 0:
            if self.utils.is_food_critical_for_action(300, self.agent.inventory) == True:
                self.command.send_command("Incantation")
            else:
                self.pathfinding.search_and_take_items(["food"], self.agent.vision)
        else:
            self.pathfinding.search_and_take_items(necessary_ressources, self.agent.vision)

    def act_pushing(self):
        """Eject
        Params:
            - Aucun parametres
        Retourne:
            - Ne return rien
        """
        self.command.send_command("Eject")
        
    def act_feeder(self):
        """Jette la nourriture au sol jusqu'à ce qu'il n'en reste que 3 dans l'inventaire.
        """

        food_count = self.agent.inventory["food"]
        food_a_jeter = food_count - 3

        if food_a_jeter > 0:
            for _ in range(food_a_jeter):
                self.command.send_command("Set food")
