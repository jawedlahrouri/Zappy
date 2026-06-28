#!/usr/bin/python3

from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_state import AgentState

logger = ZappyLogger.get_logger("AgentRole")

class AgentRole(Enum): # enum pour representer les différents roles que les drone/clone adopterons
    QUEEN = 1
    PEASANT = 4

class RoleTask:
    def __init__(self, agent):
        self.agent = agent # Référence à l'agent principal pour accéder à ses modules et données.

    def act_peasant(self):
        """
        Etats possible : BEGINNER, FARMING, FEEDER

        BEGINNER : quand il est sous le niveau 3 pour le reach le plus rapidement possible
        FARMING : qui vas agir sur une ressource en particulier
        FEEDER : vas apporter de la nouriture dans une direction precise (k)
        """
        if self.agent.level < 3:
            beginner_ids = [aid for aid, info in self.agent.team_roster.items() if info.get("level", 1) < 3 and aid != 9999] + [self.agent.agent_id]
            if self.agent.agent_id == max(beginner_ids):
                current_team_size = len(self.agent.team_roster) + 1
                needed = self.agent.population_target - current_team_size - self.agent.eggs_laid
                if needed > 0 and self.agent.state not in (AgentState.GROUPING, AgentState.ELEVATION):
                    if self.agent.inventory.get('food', 0) > 3:
                        self.agent.utils.fork()
        food_level = self.agent.inventory.get('food', 0)
        if self.agent.level >= 3 and self.agent.utils.is_grouping() and food_level >= 30:
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
            return
        if self.agent.state == AgentState.GROUPING:
            if self.agent.grouper.role == 'ROYAL_FOLLOWER':
                logger.info(f"Peasant lvl{self.agent.level} : ROYAL_FOLLOWER, je reste en position.")
                self.agent.comportment.act_grouping()
                return
            if food_level < 30:
                logger.info(f"Peasant lvl{self.agent.level} : Pas assez de food, quitte le grouping pour farmer.")
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
            else:
                self.agent.comportment.act_grouping()
            return
        elif self.agent.state == AgentState.ELEVATION:
            if self.agent.grouper.role == 'ROYAL_FOLLOWER':
                self.agent.comportment.act_elevating()
                return
            self.agent.comportment.act_elevating()
            return
        if self.agent.level < 3:
            if self.agent.utils.verify_food():
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
            else:
                self.agent.state = AgentState.BEGINNER
                self.agent.comportment.act_beginner()
            return
        food_level = self.agent.inventory.get('food', 0)
        if food_level < 50:
            self.agent.state = AgentState.FARMING
            self.agent.comportment.act_farming('food')
        else:
            self.agent.state = AgentState.WAIT
            self.agent.comportment.act_wait()

    
    def act_queen(self):
        """
        Fonction en charge de l'action de la reine.
        PARAMS:
            -self: instance de l'agent
        RETURNS:
            -None
        """

        if self.agent.state == AgentState.ELEVATION:
            result = self.agent.comportment.act_elevating()
            if result and result != AgentState.ELEVATION:
                self.agent.state = result
            return
        missing = self.agent.farmer.get_missing_stones_for_max_level(self.agent.level, self.agent.inventory)
        if len(missing) == 0:
            current_food = self.agent.inventory.get('food', 0)
            should_farm = False
            if self.agent.state != AgentState.GROUPING:
                if current_food < 45:
                    should_farm = True
            else:
                if current_food < 30:
                    should_farm = True
            
            if should_farm:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return
            else:
                min_food = 30 if self.agent.state == AgentState.GROUPING else 45
                level_3_gardes = sum(1 for k, data in self.agent.team_roster.items() if data.get("level", 1) >= 3 and data.get("food", 0) >= min_food and k != 9999)
                if level_3_gardes >= 5:
                    self.agent.state = AgentState.GROUPING
                    grouping_msg = self.agent.comms.encode(f"{self.agent.agent_id}-{self.agent.level}", "grouping")
                    self.agent.cmd.send_command(f"Broadcast {grouping_msg}")
                    fresh_vision = self.agent.cmd.send_command('Look')
                    if fresh_vision and fresh_vision != 'dead':
                        self.agent.vision = fresh_vision
                    players_on_tile = self.agent.vision[0].count('player') if self.agent.vision else 0
                    if players_on_tile >= 6:
                        confirm_vision = self.agent.cmd.send_command('Look')
                        if confirm_vision and confirm_vision != 'dead':
                            self.agent.vision = confirm_vision
                        confirmed_players = self.agent.vision[0].count('player') if self.agent.vision else 0
                        if confirmed_players >= 6:
                            logger.info(f"Reine : {confirmed_players} joueurs confirmés ! Lancement de l'élévation !")
                            self.agent.state = AgentState.ELEVATION
                            result = self.agent.comportment.act_elevating()
                            if result and result != AgentState.ELEVATION:
                                self.agent.state = result
                        else:
                            logger.warning(f"Reine : Joueurs partis entre temps ! ({confirmed_players}/6). Attente...")
                    else:
                        logger.info(f"Reine : En attente de regroupement. Joueurs sur ma case : {players_on_tile}/6")
                else:
                    logger.info(f"Reine : En attente que 5 Gardes soient lvl 3 avec {min_food} food. Actuellement prêts: {level_3_gardes}/5")
                    current_team_size = len(self.agent.team_roster) + 1
                    needed = self.agent.population_target - current_team_size - self.agent.eggs_laid
                    if needed > 0 and self.agent.inventory.get('food', 0) > 20:
                        logger.info(f"Reine : Effectif insuffisant pour le rituel ({current_team_size}/{self.agent.population_target}). Je ponds un oeuf.")
                        self.agent.utils.fork()
                    else:
                        self.agent.state = AgentState.WAIT
                        self.agent.comportment.act_wait()
                return
        if self.agent.inventory.get('food', 0) <= 6:
            self.agent.state = AgentState.FARMING
            self.agent.comportment.act_farming('food')
            return
        self.agent.state = AgentState.FARMING
        if missing:
            self.agent.comportment.act_farming(missing[0])
        elif self.agent.inventory.get('food', 0) < 50:
            self.agent.comportment.act_farming('food')
        else:
            self.agent.state = AgentState.WAIT
            self.agent.comportment.act_wait()
