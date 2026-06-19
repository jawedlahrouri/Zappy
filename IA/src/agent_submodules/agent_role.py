#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_state import AgentState

logger = ZappyLogger.get_logger("AgentRole")

class AgentRole(Enum): # enum pour representer les différents roles que les drone/clone adopterons
    QUEEN = 1
    GUARD = 2
    PROTECTOR = 3
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
        # Gestion de la population par les peasants (beginners)
        if self.agent.level < 3:
            beginner_ids = [aid for aid, info in self.agent.team_roster.items() if info.get("level", 1) < 3 and aid != 9999] + [self.agent.agent_id]
            if self.agent.agent_id == max(beginner_ids):
                current_team_size = len(self.agent.team_roster) + 1
                needed = self.agent.population_target - current_team_size - self.agent.eggs_laid
                if needed > 0 and self.agent.state not in (AgentState.GROUPING, AgentState.ELEVATION):
                    if self.agent.inventory.get('food', 0) > 3:
                        self.agent.utils.fork()

        # Priorité 1 : vérifier si on a besoin de manger
        if self.agent.utils.verify_food():
            self.agent.state = AgentState.FARMING
            self.agent.comportment.act_farming('food')
            return
        # Priorité 2 : vérifier si la reine demande un grouping
        if self.agent.utils.is_grouping():
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
            return
        # Priorité 3 : si beginner, monter au niveau 3
        if self.agent.state == AgentState.BEGINNER:
            if self.agent.level < 3:
                self.agent.comportment.act_beginner()
            else:
                self.agent.state = AgentState.FARMING
                missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
                if missing_stones:
                    self.agent.comportment.act_farming(missing_stones[0])
                elif self.agent.inventory.get('food', 0) < 50:
                    self.agent.comportment.act_farming('food')
                else:
                    self.agent.state = AgentState.WAIT
                    self.agent.comportment.act_wait()
        elif self.agent.state == AgentState.GROUPING:
            # En grouping pré-level-3 : broadcaster needmate et chercher un partenaire
            secret_msg = self.agent.comms.encode(f"{self.agent.agent_id}-{self.agent.level}", "needmate")
            self.agent.cmd.send_command(f"Broadcast {secret_msg}")
            self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)
            # Vérifier si le grouping a abouti (on est LEADER/FOLLOWER et assez de joueurs)
            if self.agent.grouper.role == 'LEADER' and len(self.agent.vision) > 0:
                needed = self.agent.grouper.player_reqs.get(self.agent.level, 1)
                if self.agent.vision[0].count('player') == needed:
                    self.agent.state = AgentState.ELEVATION
                    self.agent.comportment.act_elevating()
        elif self.agent.state == AgentState.ELEVATION:
            self.agent.comportment.act_elevating()
        elif self.agent.state == AgentState.FARMING:
            missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
            if missing_stones:
                self.agent.comportment.act_farming(missing_stones[0])
            elif self.agent.inventory.get('food', 0) < 50:
                self.agent.comportment.act_farming('food')
            else:
                self.agent.state = AgentState.WAIT
                self.agent.comportment.act_wait()
        elif self.agent.state == AgentState.FEEDER:
            self.agent.comportment.act_feeder()
        elif self.agent.state == AgentState.WAIT:
            self.agent.comportment.act_wait()
    
    def act_queen(self):
        # Priorité 1 : vérifier la nourriture
        if self.agent.utils.verify_food():
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return
            else:
                self.agent.cmd.send_command("Take food")

        # Décret Royal : assigner les rôles si pas encore fait
        if not self.agent.decret_royal_append:
            guard_id = []
            protector_id = []
            ids_restants = []
            ids_tries = sorted(self.agent.team_roster.keys(), reverse=True)

            for agent_id in ids_tries:
                if agent_id == 9999:  # Skip la reine elle-même
                    continue
                info = self.agent.team_roster[agent_id]
                lvl = info.get("level", 1)
                if len(guard_id) < 5 and lvl >= 3:
                    guard_id.append(agent_id)
                else:
                    ids_restants.append(agent_id)
            protector_id = ids_restants[:4]

            logger.info(f"Reine : Check Décret Royal -> Gardes dispo (lvl >= 3): {len(guard_id)}/5, Protecteurs dispo: {len(protector_id)}/4 (Roster size: {len(ids_tries)})")

            if len(guard_id) >= 5:
                self.agent.decret_royal_append = True
                ids_str = "-".join(str(x) for x in guard_id + protector_id)
                self.agent.decret_royal = self.agent.comms.encode(f"{self.agent.agent_id}-{ids_str}", "royalsummon")
                logger.info(f"Reine : Décret Royal émis ! Gardes: {guard_id}, Protecteurs: {protector_id}")
        
        # Toujours diffuser le décret royal (même en GROUPING) pour que les agents gardent leurs rôles
        if self.agent.decret_royal_append and self.agent.state != AgentState.ELEVATION:
            self.agent.cmd.send_command(f"Broadcast {self.agent.decret_royal}")

        # Si on est en ELEVATION, on continue l'élévation
        if self.agent.state == AgentState.ELEVATION:
            self.agent.comportment.act_elevating()
            return

        # Si on est en GROUPING, vérifier si 6 joueurs sont réunis pour l'élévation
        if self.agent.state == AgentState.GROUPING:
            if len(self.agent.vision) > 0 and self.agent.vision[0].count('player') >= 6:
                logger.info(f"Reine : 6 joueurs réunis ! Lancement de l'élévation !")
                self.agent.state = AgentState.ELEVATION
                self.agent.comportment.act_elevating()
                return
            # Pas encore 6 joueurs : broadcaster directement le message "grouping"
            # (au lieu de passer par process_grouping qui ne marche pas pour la reine)
            grouping_msg = self.agent.comms.encode(f"{self.agent.agent_id}-{self.agent.level}", "grouping")
            self.agent.cmd.send_command(f"Broadcast {grouping_msg}")
            logger.info(f"Reine : En attente de regroupement. Joueurs sur ma case : {self.agent.vision[0].count('player') if len(self.agent.vision) > 0 else 0}/6")
            return

        # Vérifier si les ressources globales sont suffisantes pour le Bus
        missing = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
        if len(missing) == 0:
            if self.agent.utils.is_food_critical_for_action(3000, self.agent.inventory):
                # Assez de pierres ET assez de food → passer en GROUPING
                logger.info(f"Reine : Ressources suffisantes ! Passage en mode GROUPING.")
                self.agent.state = AgentState.GROUPING
                grouping_msg = self.agent.comms.encode(f"{self.agent.agent_id}-{self.agent.level}", "grouping")
                self.agent.cmd.send_command(f"Broadcast {grouping_msg}")
                return
            else:
                # Assez de pierres mais pas assez de food → farmer de la food
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return

        # Par défaut : farmer les pierres manquantes
        self.agent.state = AgentState.FARMING
        missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
        if missing_stones:
            self.agent.comportment.act_farming(missing_stones[0])
        elif self.agent.inventory.get('food', 0) < 50:
            self.agent.comportment.act_farming('food')
        else:
            self.agent.state = AgentState.WAIT
            self.agent.comportment.act_wait()

    def act_guard(self):
        # Priorité 1 : vérifier la nourriture
        if self.agent.utils.verify_food():
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return
            else:
                self.agent.cmd.send_command("Take food")
        # Vérifier les ordres de la reine
        if self.agent.utils.is_elevation():
            self.agent.state = AgentState.ELEVATION
            self.agent.comportment.act_elevating()
        elif self.agent.utils.is_grouping():
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
        else:
            # Farmer les pierres manquantes
            self.agent.state = AgentState.FARMING
            missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
            if missing_stones:
                self.agent.comportment.act_farming(missing_stones[0])
            elif self.agent.inventory.get('food', 0) < 50:
                self.agent.comportment.act_farming('food')
            else:
                self.agent.state = AgentState.WAIT
                self.agent.comportment.act_wait()
    
    def act_protector(self):
        # Priorité 1 : vérifier la nourriture
        if self.agent.utils.verify_food():
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return
            else:
                self.agent.grouper._move_towards_sound(self.agent.k_queen)
                self.agent.cmd.send_command("Take food")
        # Vérifier les ordres de la reine
        if self.agent.utils.is_elevation():
            self.agent.state = AgentState.PUSHING
            self.agent.comportment.act_pushing()
        elif self.agent.utils.is_grouping():
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
        else:
            # Farmer les pierres manquantes
            self.agent.state = AgentState.FARMING
            missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
            if missing_stones:
                self.agent.comportment.act_farming(missing_stones[0])
            elif self.agent.inventory.get('food', 0) < 50:
                self.agent.comportment.act_farming('food')
            else:
                self.agent.state = AgentState.WAIT
                self.agent.comportment.act_wait()
