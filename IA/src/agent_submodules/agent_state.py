#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentState")

class AgentState(Enum): # Enum pour représenter les différents états de comportement de l'agent.
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4
    FORKING = 5

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

        missing = self.agent.farmer.get_missing_stones(self.agent.level, self.agent.inventory)
        if missing:
            success = self.agent.pathfinder.search_and_take_items(missing, self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        else:
            self.agent.pathfinder.explore(self.agent.level)
        return AgentState.FARMING

    def act_elevating(self):
        """Effectue les actions nécessaires pour tenter une incantation, en fonction du rôle de l'agent dans le regroupement (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour l'incantation."""

        logger.info(f"agent  : Mode ÉLÉVATION (Niveau {self.agent.level}) : préparation du rituel.")
        if self.agent.level == 1 or self.agent.grouper.role == 'LEADER':
            required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
            for stone_name, amount in required_stones.items():
                for _ in range(amount):
                    self.agent.cmd.set_object(stone_name)
            
            success = self.agent.cmd.incantation()
            if success:
                self.agent.level += 1
                logger.info(f"agent : L'Incantation a réussie ! Passage au Niveau {self.agent.level}.")
            else:
                logger.warning(f"agent : L'Incantation a échouée. Vérifiez les ressources et les alliés présents.")
        elif self.agent.grouper.role == 'FOLLOWER':
            success = self.agent.cmd.wait_for_incantation_follower()
            if success:
                self.agent.level += 1
                logger.info(f"agent : L'Incantation a réussie ! Passage au Niveau {self.agent.level}.")
        
        self.agent.grouper.role = None
        return AgentState.SURVIVAL

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

        is_grouped = self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)            
        if is_grouped:
            logger.info(f"agent  : Regroupement validé ! Passage en phase d'Incantation.")
            return AgentState.ELEVATION
        return AgentState.GROUPING
    
    def act_forking(self):
        """Effectue les actions de clonage pour créer un nouvel agent.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le clonage."""

        success = self.agent.cmd.fork()
        if success:
            self.agent.utils.launch_clone()
        return AgentState.SURVIVAL