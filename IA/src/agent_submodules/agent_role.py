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
                self.agent.comportment.act_farming('food')
        elif self.agent.state == AgentState.GROUPING:
            # En grouping pré-level-3 : broadcaster needmate et chercher un partenaire
            secret_msg = self.agent.comms.encode(f"lv{self.agent.level}-{self.agent.agent_id}", "needmate")
            self.agent.cmd.send_command(f"Broadcast {secret_msg}")
            self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)
            # Vérifier si le grouping a abouti (on est LEADER/FOLLOWER et assez de joueurs)
            if self.agent.grouper.role == 'LEADER' and len(self.agent.vision) > 0:
                needed = self.agent.grouper.player_reqs.get(self.agent.level, 1)
                if self.agent.vision[0].count('player') >= needed:
                    self.agent.state = AgentState.ELEVATION
                    self.agent.comportment.act_elevating()
        elif self.agent.state == AgentState.ELEVATION:
            self.agent.comportment.act_elevating()
        elif self.agent.state == AgentState.FARMING:
            self.agent.comportment.act_farming('food')
    
    def act_queen(self):
        # Priorité 1 : vérifier la nourriture
        if self.agent.utils.verify_food():
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
                return
            else:
                self.agent.cmd.send_command("Take food")

        # Gestion de la population : pondre des oeufs si besoin
        if len(self.agent.team_roster) <= self.agent.population_target and self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
            self.agent.utils.fork()

        # Vérifier les oeufs éclos et lancer des clones
        if self.agent.eggs_laid >= 1:
            nb_mature_egg = self.agent.cmd.send_command('Connect_nbr')
            if isinstance(nb_mature_egg, int) and nb_mature_egg >= 1:
                self.agent.utils.launch_clone()
                self.agent.eggs_laid -= nb_mature_egg

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

            # On ne peut émettre le décret que si on a assez de gardes et protecteurs
            if len(guard_id) >= 5 and len(protector_id) >= 4:
                self.agent.decret_royal_append = True
                ids_str = "-".join(str(x) for x in guard_id + protector_id)
                self.agent.decret_royal = self.agent.comms.encode(f"{self.agent.agent_id}-{ids_str}", "royalsummon")
                logger.info(f"Reine : Décret Royal émis ! Gardes: {guard_id}, Protecteurs: {protector_id}")
        
        # Diffuser le décret royal si pas en grouping/elevation
        if self.agent.decret_royal_append and self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
            self.agent.cmd.send_command(f"Broadcast {self.agent.decret_royal}")

        # Vérifier si les ressources globales sont suffisantes pour le Bus
        missing = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
        if len(missing) == 0 and self.agent.utils.is_food_critical_for_action(3000, self.agent.inventory):
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
            return

        # Vérifier si assez de joueurs pour lancer l'élévation
        if self.agent.state == AgentState.GROUPING and len(self.agent.vision) > 0:
            if self.agent.vision[0].count('player') >= 6:
                self.agent.state = AgentState.ELEVATION
                self.agent.comportment.act_elevating()
                return

        # Par défaut : farmer les pierres manquantes
        if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
            self.agent.state = AgentState.FARMING
            missing_stones = self.agent.farmer.get_missing_stones_for_max_level(3, self.agent.global_inventory)
            if missing_stones:
                self.agent.comportment.act_farming(missing_stones[0])
            else:
                self.agent.comportment.act_farming('food')

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
            else:
                self.agent.comportment.act_farming('food')
    
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
            else:
                self.agent.comportment.act_farming('food')
