#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger
from src.agent_submodules.agent_state import AgentState, StateComportment
from src.utils import utils
from src.agent_submodules.farm import Farmer
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.grouping import Grouper

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
        self.pathfinding = Pathfinder
        self.grouping = Grouper
        self.comportment = StateComportment(self) # Comportement de l'agent selon son état (survie, farming, grouping, etc).


    def act_peasant(self):
        """
        Etats possible : BEGINNER, FARMING, FEEDER

        BEGINNER : quand il est sous le niveau 3 pour le reach le plus rapidement possible
        FARMING : qui vas agir sur une ressource en particulier
        FEEDER : vas apporter de la nouriture dans une direction precise (k)
        """
        if self.utils.verify_food() == True:
            self.agent.state = AgentState.FARMING
            self.comportment.act_farming('food')
        if self.agent.state == AgentState.BEGINNER:
            if self.agent.level < 3:
                self.comportment.act_beginner()
            else:
                self.agent.state = AgentState.FARMING
                self.comportment.act_farming('food')
        if self.utils.is_grouping() == True:
            self.agent.state = AgentState.GROUPING
            self.comportment.act_grouping()
    
    def act_queen(self):
        if self.utils.verify_food() == True:
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.comportment.act_farming('food')
            else:   #TODO: ajouter une loop pour que la queen take tant qu'elle n'a pas atteint son seuil de nourriture
                self.command.send_command("Take food")
        if len(self.agent.team_roster) <= self.agent.population_target and (self.agent.state != AgentState.GROUPING or self.agent.state != AgentState.ELEVATION):
            self.utils.fork()
        if self.agent.egg_laid >= 1:
            nb_mature_egg = self.agent.cmd.send_command('connect_nbr')
            if nb_mature_egg >= 1:
                self.utils.launch_clone()
            egg_laid -= nb_mature_egg
        if self.farm.get_missing_stones_for_max_level().size() == 0 and self.utils.is_food_critical_for_action(3000, self.agent.inventory) == True:
            self.agent.state = AgentState.GROUPING
            self.comportment.act_grouping()
        if self.agent.vision[0].count('player') > 6 and self.agent.state == AgentState.GROUPING:
            self.agent.state = AgentState.ELEVATION
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
            self.agent.decret_royal = self.agent.comms.encode(f"{self.agent.agent_id}-{guard_id[0]}-{guard_id[1]}-{guard_id[2]}-{guard_id[3]}-{guard_id[4]}-{protector_id[0]}-{protector_id[1]}-{protector_id[2]}-{protector_id[3]}", "royalsummon")
        
        if self.agent.decret_royal_append == True and self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
            self.agent.cmd.send_command(f"broadcast {self.agent.decret_royal}")
            self.agent.state = AgentState.FARMER
            self.comportment.act_farmer()

    def act_guard(self):
        if self.utils.verify_food() == True:
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.comportment.act_farming('food')
            else:
                self.command.send_command("Take food") # TODO: ajouter une loop pour que le guard take tant qu'il n'a pas atteint son seuil de nourriture
        if self.utils.is_grouping() == True:
            self.agent.state = AgentState.GROUPING
            self.comportment.act_grouping()
        elif self.utils.is_elevation() == True:
            self.agent.state = AgentState.ELEVATION
            self.comportment.act_elevation()
        else:
            self.agent.state = AgentState.FARMING
            self.comportment.act_farmer()
    
    def act_protector(self):
        if self.utils.verify_food() == True:
            if self.agent.state != AgentState.GROUPING and self.agent.state != AgentState.ELEVATION:
                self.agent.state = AgentState.FARMING
                self.comportment.act_farming('food')
            else:
                self.grouping._move_towards_sound(self.agent.k_queen) # TODO: ajouter une loop pour que le protector take tant qu'il n'a pas atteint son seuil de nourriture
                self.command.send_command("Take food")
        if self.utils.is_grouping() == True:
            self.agent.state = AgentState.GROUPING
            self.comportment.act_grouping()
        elif self.utils.is_elevation() == True:
            self.agent.state = AgentState.PUSHING
            self.comportment.act_pushing()
        else:
            self.agent.state = AgentState.FARMING
            self.comportment.act_farmer()
