#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_role import AgentRole


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
    WAIT = 10

class StateComportment:
    def __init__(self, agent):
        self.agent = agent # Référence à l'agent principal pour accéder à ses modules et données.

    def _clean_and_set_stones(self, required_stones):
        """Nettoie la case et pose exactement le nombre de pierre necessaire pour l'incantation
        Params:
            - required_stones (dict): Dictionnaire des pierres requises pour l'incantation.
        Retourne:
            - bool: True si l'incantation a été réussie, False sinon.
        """
        vision = self.agent.cmd.send_command("Look")
        if vision:
            self.agent.vision = vision
        tile_contents = self.agent.vision[0] if self.agent.vision else []
        all_items = ['food', 'linemate', 'deraumere', 'sibur', 'mendiane', 'phiras', 'thystame']
        for item in all_items:
            on_ground = tile_contents.count(item)
            for _ in range(on_ground):
                self.agent.cmd.send_command(f"Take {item}")
        for stone_name, amount in required_stones.items():
            if amount == 0:
                continue
            for _ in range(amount):
                self.agent.cmd.send_command(f"Set {stone_name}")
        return True

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

    def act_farming(self, item):
        """Effectue les actions de farming pour collecter les ressources nécessaires à l'incantation.\n
        Params:
            -item (str): Le nom de l'item à collecter.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour farming."""
        
        if item == 'food':
            success = self.agent.pathfinder.search_and_take_items(['food'], self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
            return AgentState.FARMING

        if self.agent.role == AgentRole.PEASANT:
            success = self.agent.pathfinder.search_and_take_items([item], self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        if self.agent.role == AgentRole.QUEEN:
            missing = self.agent.farmer.get_missing_stones_for_max_level(self.agent.level, self.agent.inventory)
            if missing:
                success = self.agent.pathfinder.search_and_take_items(missing, self.agent.vision)
                if not success:
                    self.agent.pathfinder.explore(self.agent.level)
            else:
                success = self.agent.pathfinder.search_and_take_items(['food'], self.agent.vision)
                if not success:
                    self.agent.pathfinder.explore(self.agent.level)
        return AgentState.FARMING

    def act_elevating(self):
        """Effectue les actions nécessaires pour tenter une incantation, en fonction du rôle de l'agent dans le regroupement (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour l'incantation."""

        from src.agent_submodules.agent_role import AgentRole

        if self.agent.role == AgentRole.QUEEN or self.agent.grouper.role == 'ROYAL_FOLLOWER':
            if self.agent.role == AgentRole.QUEEN:
                required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                if not self._clean_and_set_stones(required_stones):
                    logger.warning("Reine : Nettoyage de case échoué, incantation reportée.")
                    return AgentState.ELEVATION
                success = self.agent.cmd.send_command("Incantation")
                if success:
                    self.agent.level += 1
                    logger.info(f"Reine : Incantation réussie ! Niveau actuel : {self.agent.level}")
                    if self.agent.level < 8:
                        return self.act_elevating()
                else:
                    logger.warning("Reine : Incantation échouée (ko ou eject), retour au regroupement.")
                return AgentState.GROUPING
            else:
                success = self.agent.cmd.wait_for_incantation_follower()
                if success:
                    self.agent.level += 1
                    logger.info(f"{self.agent.role.name.capitalize()} : Incantation réussie ! Niveau actuel : {self.agent.level}")
                    if self.agent.level < 8:
                        return self.act_elevating()
                return AgentState.GROUPING
        else:
            if self.agent.level == 1 or self.agent.grouper.role == 'LEADER':
                required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                if not self._clean_and_set_stones(required_stones):
                    logger.warning("Nettoyage de case échoué, abandon de l'incantation.")
                    self.agent.cmd.send_command("Forward")
                    self.agent.state = AgentState.BEGINNER
                    self.agent.grouper.role = None
                    return AgentState.BEGINNER
                success = self.agent.cmd.send_command("Incantation")
                if success:
                    self.agent.level += 1
                    self.agent.state = AgentState.BEGINNER
                else:
                    logger.warning("Incantation a échoué (ko reçu). La case est mauvaise, fuite...")
                    for stone_name, amount in required_stones.items():
                        for _ in range(amount):
                            self.agent.cmd.send_command(f"Take {stone_name}")
                    self.agent.cmd.send_command("Forward")
                    self.agent.state = AgentState.BEGINNER
            elif self.agent.grouper.role == 'FOLLOWER':
                success = self.agent.cmd.wait_for_incantation_follower()
                if success:
                    self.agent.level += 1
                self.agent.state = AgentState.BEGINNER
            self.agent.grouper.role = None
            return AgentState.SURVIVAL

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

        if self.agent.inventory.get('food', 0) <= self.agent.food_minimum and self.agent.level < 3:
            logger.warning("Grouping abandonné : nourriture critique !")
            self.agent.grouper.role = None
            self.agent.state = AgentState.SURVIVAL
            return AgentState.SURVIVAL
        self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)
        return AgentState.GROUPING

    def act_beginner(self):
        """Effectue les actions attendus de la part d'un bot qui vient d'arriver.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """
        if self.agent.level >= 3:
            self.agent.state = AgentState.FARMING
            return AgentState.FARMING
        
        necessary_ressources = self.agent.farmer.get_missing_stones(self.agent.level, self.agent.inventory)
        if len(necessary_ressources) == 0:
            if self.agent.utils.is_food_critical_for_action(300, self.agent.inventory):
                if self.agent.level == 1:
                    required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                    if not self._clean_and_set_stones(required_stones):
                        logger.warning("Beginner : Nettoyage de case échoué, abandon.")
                        self.agent.cmd.send_command("Forward")
                        return AgentState.BEGINNER
                    success = self.agent.cmd.send_command("Incantation")
                    if success:
                        self.agent.level += 1
                        logger.info(f"Beginner : Incantation réussie ! Niveau actuel : {self.agent.level}")
                        ping_content = f"{self.agent.agent_id}-{self.agent.level}-{self.agent.inventory.get('food', 0)}-{self.agent.inventory.get('linemate', 0)}-{self.agent.inventory.get('deraumere', 0)}-{self.agent.inventory.get('sibur', 0)}-{self.agent.inventory.get('mendiane', 0)}-{self.agent.inventory.get('phiras', 0)}-{self.agent.inventory.get('thystame', 0)}"
                        secret_ping = self.agent.comms.encode(ping_content, "ping")
                        self.agent.cmd.send_command(f"Broadcast {secret_ping}")
                else:
                    if self.agent.inventory.get('food', 0) < self.agent.food_safe:
                        logger.info(f"Beginner lvl{self.agent.level} : Pas assez de food pour grouping, farming d'abord.")
                        self.agent.pathfinder.search_and_take_items(["food"], self.agent.vision)
                        return AgentState.BEGINNER
                    self.agent.state = AgentState.GROUPING
                    secret_msg = self.agent.comms.encode(f"{self.agent.agent_id}-{self.agent.level}", "needmate")
                    self.agent.cmd.send_command(f"Broadcast {secret_msg}")
                    logger.info(f"Beginner lvl{self.agent.level} : Recherche d'un partenaire pour incantation (needmate broadcasté)")
                    self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)
                    return AgentState.GROUPING
            else:
                self.agent.pathfinder.search_and_take_items(["food"], self.agent.vision)
        else:
            success = self.agent.pathfinder.search_and_take_items(necessary_ressources, self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        return AgentState.BEGINNER


    def act_feeder(self):
        """Jette de la nourriture au sol jusqu'à etre à sec
        PARAMS:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour survivre.
        """

        food_count = self.agent.inventory.get("food", 0)
        food_a_jeter = food_count - 3

        if food_a_jeter > 0:
            for _ in range(food_a_jeter):
                self.agent.cmd.send_command("Set food")
        return AgentState.FEEDER

    def act_wait(self):
        """Etat d'attente en cas de feeder qui a trop de bouffe
        """
        if self.agent.utils.is_grouping():
            self.agent.state = AgentState.GROUPING
        else:
            self.agent.cmd.send_command("Inventory")
            return AgentState.WAIT
