#!/usr/bin/python3
from src.Ai_agent import AgentState

class StateComportment:
    def __init__(self, agent):
        self.agent = agent

    #cherche de la nourriture pour eviter de mourrir
    def act_survival(self):
        success = self.agent.pathfinder.search_and_take_items(['food'], self.agent.vision)
        if not success:
            self.agent.pathfinder.explore(self.agent.level)
        return AgentState.SURVIVAL

    #cherche les ressources necessaire à son amélioration    
    def act_farming(self):
        missing = self.agent.farmer.get_missing_stones(self.agent.level, self.agent.inventory)
        if missing:
            success = self.agent.pathfinder.search_and_take_items(missing, self.agent.vision)
            if not success:
                self.agent.pathfinder.explore(self.agent.level)
        else:
            self.agent.pathfinder.explore(self.agent.level)
        return AgentState.FARMING

    #tente de s'elever en appelant ses mates
    def act_elevating(self):
        print(f"[Agent] Mode ÉLÉVATION (Niveau {self.agent.level}) : préparation du rituel.")
        if self.agent.level == 1 or self.agent.grouper.role == 'LEADER':
            required_stones = self.agent.farmer.requirements.get(self.agent.level, {})
            for stone_name, amount in required_stones.items():
                for _ in range(amount):
                    self.agent.cmd.set_object(stone_name)
            
            success = self.agent.cmd.incantation()
            if success:
                self.agent.level += 1
                print(f"[Agent] SUCCÈS, passe au Niveau {self.agent.level} !")
            else:
                print("[Agent] Échec de l'incantation.")        
        elif self.agent.grouper.role == 'FOLLOWER':
            success = self.agent.cmd.wait_for_incantation_follower()
            if success:
                self.agent.level += 1
                print(f"[Agent] FOLLOWER passe au Niveau {self.agent.level} !")
        
        self.agent.grouper.role = None
        return AgentState.SURVIVAL

    #tente de rejoindre le signal qu'on lui envoie    
    def act_grouping(self):
        is_grouped = self.agent.grouper.process_grouping(self.agent.level, self.agent.agent_id, self.agent.vision)            
        if is_grouped:
            print("[Agent] Regroupement validé ! Passage en phase d'Incantation.")
            return AgentState.ELEVATION
        return AgentState.GROUPING
    
    #donne naissance à un bébé
    def act_forking(self):
        success = self.agent.cmd.fork()
        if success:
            self.agent.utils.launch_clone()
        return AgentState.SURVIVAL