#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_state import AgentState
from src.utils import utils
from src.agent_submodules.farm import Farmer

logger = ZappyLogger.get_logger("AgentRole")

class AgentRole(Enum): # enum pour representer les différents roles que les drone/clone adopterons
    QUEEN = 1
    GUARD = 2
    PROTECTOR = 3
    PEASANT = 4

class RoleTask:
    def __init__(self, agent):
        self.agent = agent # Référence à l'agent principal pour accéder à ses modules et données.
        self.utils = utils
        self.farm = Farmer

    def act_peasant(self):
        """
        Etats possible : BEGINNER, FARMING, FEEDER

        BEGINNER : quand il est sous le niveau 2 pour le reach le plus rapidement possible
        FARMING : qui vas agir sur une ressource en particulier
        FEEDER : vas apporter de la nouriture dans une direction precise (k)
        """
        if self.agent.state == AgentState.BEGINNER:
            if self.agent.level < 2:
                self.agent.comportment.act_beginner()
            else:
                self.agent.state = AgentState.FARMING
                self.agent.comportment.act_farming('food')
        k, message = self.agent.analyze_calls()
        message_parts = message.split('_')
        if message_parts[1] == 'hungry' or message_parts[2] == 'grouping':
            self.agent.state = AgentState.FEEDER
            self.agent.comportment.act_feeder(k)
    
    def act_queen(self):
        if len(self.agent.team_roster) <= self.agent.population_target:
            self.utils.fork()
        if self.agent.egg_laid >= 1:
            nb_mature_egg = self.agent.cmd.send_command('connect_nbr')
            if nb_mature_egg >= 1:
                self.utils.launch_clone()
            egg_laid -= nb_mature_egg
        if self.farm.get_missing_stones_for_max_level().size() == 0 and self.utils.is_food_critical_for_action(3000, self.agent.inventory) == True:
            self.agent.role = AgentState.GROUPING
            self.agent.comportment.act_grouping()
        if self.agent.role == AgentState.GROUPING:
            self.agent.role = AgentState.ELEVATION
            self.agent.comportement.act_elevation()
        if self.agent.decret_royal_append == False:
            guard_id = []
            protector_id = []
            ids_restants = []
            ids_tries = sorted(self.agent.team_roster.keys(), reverse=True)

            for agent_id in ids_tries:
                lvl = self.agent.team_roster[agent_id]["level"]
                if len(guard_id) < 5 and lvl >= 3:
                    guard_id.append(agent_id)
                else:
                    ids_restants.append(agent_id)
                protector_id = ids_restants[:4]
            
            self.agent.decret_royal_append = True
            self.agent.decret_royal = self.agent.comms.encode("{self.agent.agent_id}-{guard_id[0]}-{guard_id[1]}-{guard_id[2]}-{guard_id[3]}-{guard_id[4]}-{protector_id[0]}-{protector_id[1]}-{protector_id[2]}-{protector_id[3]}", "royalsummon")
        
        if self.decret_royal_append == True:
            self.agent.cmd.send_command("broadcast {self.agent.decret_royal}")
            self.agent.state = AgentState.FARMER
            self.agent.comportment.act_farmer()
    
    def act_guard(self):
        if self.utils.is_grouping() == True:
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
        elif self.utils.is_elevation() == True:
            self.agent.state = AgentState.ELEVATION
            self.agent.comportment.act_elevation()
        else:
            self.agent.state = AgentState.FARMING
            self.agent.comportment.act_farmer()
    
    def act_protector(self):
        if self.utils.is_grouping() == True:
            self.agent.state = AgentState.GROUPING
            self.agent.comportment.act_grouping()
        elif self.utils.is_elevation() == True:
            self.agent.state = AgentState.PUSHING
            self.agent.comportment.act_pushing()
        else:
            self.agent.state = AgentState.FARMING
            self.agent.comportment.act_farmer()
