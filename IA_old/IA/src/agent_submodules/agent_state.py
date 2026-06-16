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
    BALISE = 6
    BEGINER = 7
    FEEDER = 8

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

        logger.info(f"agent : Mode ÉLÉVATION (Niveau {self.agent.level}) : préparation du rituel.")
        if self.agent.level == 1 or self.agent.grouper.role == 'LEADER':
            required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
            for stone_name, amount in required_stones.items():
                for _ in range(amount):
                    self.agent.cmd.send_command(f"Set {stone_name}")
            
            success = self.agent.cmd.send_command("Incantation")
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
            logger.info(f"agent : Regroupement validé ! Passage en phase d'Incantation.")
            return AgentState.ELEVATION
        return AgentState.GROUPING
    
    def act_forking(self):
        """Effectue les actions de clonage pour créer un nouvel agent.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le clonage."""

        success = self.agent.cmd.send_command("Fork")
        if success:
            self.agent.eggs_laid += 1
            logger.info("Reine : Un oeuf a été pondu. En attente de l'éclosion (600 TU)...")
        return AgentState.SURVIVAL

    def act_balise(self):
        """Effectue les actions de la balise qui vas servir de (0, 0) dans les coordonées.
        Params:
            -Aucun parametre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester a un endroit precis pour servir de (0,0) pour les autres."""
        if self.agent.is_alive:
            food = self.agent.inventory.get('food', 0)
            if len(self.agent.vision) > 0 and self.agent.vision[0].count('food') > 0:
                self.agent.cmd.send_command("Take food")
                logger.info(f"La BALISE (ID {self.agent.agent_id}) se nourrit de la nourriture déposée.")
                
            current_population = len(self.agent.team_roster)
            if current_population < self.agent.population_target:
                is_queen = True
                for ally_id in self.agent.team_roster.keys():
                    if ally_id > self.agent.agent_id:
                        is_queen = False
                        break
                if is_queen and food > 5:
                    if not self.agent.utils.is_food_critical_for_action(42, self.agent.inventory, self.agent.food_minimum):
                        logger.info(f"La BALISE (ID {self.agent.agent_id}) est la REINE et initie un Fork pour recruter un nouveau drone. Pop actuelle: {current_population}/{self.agent.population_target}.")
                        success = self.agent.cmd.send_command("Fork")
                        if success:
                            self.agent.utils.launch_clone()
                            return AgentState.BALISE

            secret_msg = self.agent.comms.encode(str(self.agent.agent_id), "balise")
            self.agent.cmd.send_command(f"Broadcast {secret_msg}")
        return AgentState.BALISE
        
    def act_feeder(self):
        """Effectue les actions de ravitaillement de la balise.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester."""
        if self.agent.balise_k is None:
            return AgentState.SURVIVAL
            
        if self.agent.balise_k == 0:
            food = self.agent.inventory.get('food', 0)
            if food > self.agent.food_safe:
                self.agent.cmd.send_command("Set food")
                logger.info(f"Ouvrière (ID {self.agent.agent_id}) dépose de la nourriture pour la balise.")
                return AgentState.FEEDER
            else:
                return AgentState.SURVIVAL
        else:
            self.agent.grouper._move_towards_sound(self.agent.balise_k)
            self.agent.balise_k = None
            return AgentState.FEEDER
    
    def act_beginer(self):
        """Effectue les actions attendus de la part d'un bot qui vient d'arriver.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """
        if self.agent.balise_k is not None and self.agent.balise_k > 0:
            self.agent.grouper._move_towards_sound(self.agent.balise_k)
            self.agent.balise_k = None
            return AgentState.BEGINER
        
        elif self.agent.heard_balise:
            self.agent.cmd.send_command("Inventory")
            return AgentState.BEGINER
        
        else:
            if not self.agent.ping_sent:
                self.agent.cmd.send_command("Broadcast TEAM_PING")
                self.agent.ping_sent = True
                return AgentState.BEGINER

            elif self.agent.wait_counter < 3:
                self.agent.cmd.send_command("Inventory")
                self.agent.wait_counter += 1
                return AgentState.BEGINER
            
            
            else:
                self.agent.state = AgentState.BALISE
                logger.info(f"L'agent {self.agent.agent_id} a terminé sa phase d'initialisation et devient une BALISE.") # a voir jawed pour le cryptage
                return AgentState.BALISE
        