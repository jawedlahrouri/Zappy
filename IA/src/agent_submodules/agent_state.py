#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger

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

        if self.agent.level == 1 or self.agent.grouper.role == 'LEADER' or self.agent.role == self.agent.AgentRole.QUEEN:
            required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
            for stone_name, amount in required_stones.items():
                for _ in range(amount):
                    self.agent.cmd.send_command(f"Set {stone_name}")               
            success = self.agent.cmd.send_command("Incantation")
            if success:
                self.agent.level += 1
            if self.agent.role == self.agent.AgentRole.QUEEN and success and self.agent.level >= 3:
                self.act_elevating()
        elif self.agent.grouper.role == 'FOLLOWER' or self.agent.role == self.agent.AgentRole.GUARD:
            success = self.agent.cmd.wait_for_incantation_follower()
            if success:
                self.agent.level += 1
            if self.agent.role == self.agent.AgentRole.GUARD and success and self.agent.level >= 3:
                self.act_elevating()
        self.agent.grouper.role = None
        return AgentState.SURVIVAL
        #TODO: retaffer logique elevation quand moins fatigué 

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

    def act_beginner(self):
        """Effectue les actions attendus de la part d'un bot qui vient d'arriver.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """
    
    def act_pushing(self):
        """Eject
        Params:
            - Aucun parametres
        Retourne:
            - Ne return rien
        """
    
    def act_leader(self):
        """Effectue les actions attendus de la part d'un bot qui est leader.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """

    def act_follower(self):
        """Effectue les actions attendus de la part d'un bot qui est follower.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """