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

    def act_farming(self, item):
        """Effectue les actions de farming pour collecter les ressources nécessaires à l'incantation.\n
        Params:
            -item (str): Le nom de l'item à collecter.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour farming."""
        
        from src.agent_submodules.agent_role import AgentRole
        if self.agent.role == AgentRole.PEASANT or self.agent.role == AgentRole.GUARD:
            success = self.agent.pathfinder.search_and_take_items([item], self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        if self.agent.role == AgentRole.PROTECTOR or self.agent.role == AgentRole.QUEEN:
            missing = self.agent.farmer.get_missing_stones_for_max_level(self.agent.level, self.agent.global_inventory)
            if missing:
                success = self.agent.pathfinder.search_and_take_items(missing, self.agent.vision)
                if not success:
                    self.agent.pathfinder.explore(self.agent.level)
            else:
                # Toutes les pierres sont collectées, on farme de la food en attendant
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
            # Phase Bus : la Reine pose les pierres, les Gardes attendent
            if self.agent.role == AgentRole.QUEEN:
                # La Reine pose les pierres nécessaires pour le niveau courant
                required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                for stone_name, amount in required_stones.items():
                    for _ in range(amount):
                        self.agent.cmd.send_command(f"Set {stone_name}")
                success = self.agent.cmd.send_command("Incantation")
                if success:
                    self.agent.level += 1
                    logger.info(f"Reine : Incantation réussie ! Niveau actuel : {self.agent.level}")
                    if self.agent.level < 8:
                        return self.act_elevating()  # Chaîner les incantations
            elif self.agent.role == AgentRole.GUARD:
                # Les gardes déposent toutes leurs pierres et attendent
                for stone_name, amount in self.agent.inventory.items():
                    if stone_name != 'food':
                        for _ in range(amount):
                            self.agent.cmd.send_command(f"Set {stone_name}")
                success = self.agent.cmd.wait_for_incantation_follower()
                if success:
                    self.agent.level += 1
                    logger.info(f"Garde : Incantation réussie ! Niveau actuel : {self.agent.level}")
                    if self.agent.level < 8:
                        return self.act_elevating()  # Chaîner les incantations
        return AgentState.SURVIVAL

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

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
                # Assez de food pour tenter l'incantation solo (niveau 1)
                if self.agent.level == 1:
                    required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
                    for stone_name, amount in required_stones.items():
                        for _ in range(amount):
                            self.agent.cmd.send_command(f"Set {stone_name}")
                    success = self.agent.cmd.send_command("Incantation")
                    if success:
                        self.agent.level += 1
                        logger.info(f"Beginner : Incantation réussie ! Niveau actuel : {self.agent.level}")
                else:
                    # Niveau 2+, besoin d'un partenaire → broadcaster needmate et entrer en grouping
                    self.agent.state = AgentState.GROUPING
                    # Broadcaster un needmate pour trouver un partenaire
                    secret_msg = self.agent.comms.encode(f"lv{self.agent.level}-{self.agent.agent_id}", "needmate")
                    self.agent.cmd.send_command(f"Broadcast {secret_msg}")
                    logger.info(f"Beginner lvl{self.agent.level} : Recherche d'un partenaire pour incantation (needmate broadcasté)")
                    # Vérifier si un allié a déjà répondu
                    self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)
                    return AgentState.GROUPING
            else:
                self.agent.pathfinder.search_and_take_items(["food"], self.agent.vision)
        else:
            success = self.agent.pathfinder.search_and_take_items(necessary_ressources, self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        return AgentState.BEGINNER

    def act_pushing(self):
        """Eject les intrus autour de la reine.
        Params:
            - Aucun parametres
        Retourne:
            - AgentState: l'état PUSHING
        """
        self.agent.cmd.send_command("Eject")
        return AgentState.PUSHING
        
    def act_feeder(self):
        """Jette la nourriture au sol jusqu'à ce qu'il n'en reste que 3 dans l'inventaire.
        Retourne AgentState.FEEDER."""

        food_count = self.agent.inventory.get("food", 0)
        food_a_jeter = food_count - 3

        if food_a_jeter > 0:
            for _ in range(food_a_jeter):
                self.agent.cmd.send_command("Set food")
        return AgentState.FEEDER
