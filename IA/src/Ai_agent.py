#!/usr/bin/python3

import random
import time
import sys
import subprocess
from enum import Enum
from src.command import ZappyCommand
from src.agent_submodules.pathfinding import Pathfinder
from src.agent_submodules.farm import Farmer
from src.agent_submodules.grouping import Grouper
from src.agent_submodules.stealth_comms import StealthComms

class AgentState(Enum):
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4
    FORKING = 5

class ZappyAgent:
    def __init__(self, command_interface: ZappyCommand, team_name: str):
        self.cmd = command_interface
        self.pathfinder = Pathfinder(self.cmd)
        self.farmer = Farmer()
        self.comms = StealthComms()
        self.grouper = Grouper(self.cmd, self.comms)

        self.team_name = team_name
        self.unread_messages = []
        self.agent_id = random.randint(1000, 9999)
        self.is_alive = True
        self.state = AgentState.SURVIVAL
        self.level = 1
        self.inventory = {}
        self.vision = []
        self.food_minimum = 6
        self.food_safe = 15
        self.team_roster = {}
        self.population_target = 10
        self.last_ping_time = time.time()

    def update_internal_state(self):
        if not self.cmd.network.connected:
            self.is_alive = False
            print("[Agent] Connexion perdue.")
            return
        
        response = self.cmd.look()
        if response == "dead":
            self.is_alive = False
            return
        self.vision = response
        
        response = self.cmd.inventory()
        if response == "dead":
            self.is_alive = False
            return
        self.inventory = response
        self.unread_messages = self.cmd.get_unread_broadcasts()

        self.decoded_messages = []
        for msg in self.unread_messages:
            try:
                clean_msg = msg.replace("message ", "")
                parts = clean_msg.split(",", 1)
                k = int(parts[0].strip())
                raw_text = parts[1].strip()
                decoded = self.comms.decode(raw_text)
                if decoded:
                    self.decoded_messages.append({'k': k, 'data': decoded})
            except Exception:
                continue
        current_time = time.time()
        if current_time - self.last_ping_time > 15:
            secret_msg = self.comms.encode(str(self.agent_id), "ping")
            self.cmd.broadcast_text(secret_msg)
            self.last_ping_time = current_time

    def process_radio_and_demographics(self):
        current_time = time.time()
        for msg_obj in self.decoded_messages:
            decoded = msg_obj['data']
            if decoded['command'] == 'ping':
                sender_id = int(decoded['param'])
                self.team_roster[sender_id] = current_time
            elif decoded['command'] == 'needmate':
                sender_id = int(decoded['param'].split('-')[1])
                self.team_roster[sender_id] = current_time
        dead_ids = [ally_id for ally_id, last_seen in self.team_roster.items() if current_time - last_seen > 40]
        for ally_id in dead_ids:
            print(f"[Agent] L'agent {ally_id} ne répond plus. Déclaré mort.")
            del self.team_roster[ally_id]

    def evaluate_situation(self):
        if self.state == AgentState.ELEVATION:
            return
        self.process_radio_and_demographics()
        food = self.inventory.get('food', 0)
        if food <= self.food_minimum:
            self.state = AgentState.SURVIVAL
            return
        call_k = self.grouper.analyze_calls(self.level, self.agent_id, self.decoded_messages)
        if call_k is not None:
            print(f"[Agent] Appel radio valide reçu (K={call_k}) ! On passe en GROUPING.")
            self.grouper.current_k = call_k
            self.state = AgentState.GROUPING
            return
        if self.farmer.can_elevate(self.level, self.inventory):
            if food < (self.food_minimum + 3):
                print("[Agent] Pierres OK, mais nourriture insuffisante pour le rituel. Survie !")
                self.state = AgentState.SURVIVAL
                return
            print(f"[Agent] Ressources d'élévation prêtes pour le Niveau {self.level} ! Passage en mode ÉLÉVATION.")
            if self.level == 1:
                self.state = AgentState.ELEVATION
            else:
                self.state = AgentState.GROUPING
            return
            
        current_population = len(self.team_roster) + 1
        if current_population < self.population_target:
            is_queen = True
            for ally_id in self.team_roster.keys():
                if ally_id > self.agent_id:
                    is_queen = False
                    break
            if is_queen and food > self.food_safe:
                if not self.is_food_critical_for_action(42):
                    print(f"[Agent] Je suis la REINE (ID {self.agent_id}). Pop actuelle: {current_population}/{self.population_target}.")
                    self.state = AgentState.FORKING
                    return

        if food > self.food_safe:
            self.state = AgentState.FARMING
            return
        self.state = AgentState.SURVIVAL

    def act_based_on_state(self):
        if self.state == AgentState.SURVIVAL:
            success = self.pathfinder.search_and_take_items(['food'], self.vision)
            if not success:
                self.pathfinder.explore(self.level)
        elif self.state == AgentState.FARMING:
            missing = self.farmer.get_missing_stones(self.level, self.inventory)
            if missing:
                success = self.pathfinder.search_and_take_items(missing, self.vision)
                if not success:
                    self.pathfinder.explore(self.level)
            else:
                self.pathfinder.explore(self.level)

        elif self.state == AgentState.GROUPING:
            is_grouped = self.grouper.process_grouping(self.level, self.agent_id, self.vision)            
            if is_grouped:
                print("[Agent] Regroupement validé ! Passage en phase d'Incantation.")
                self.state = AgentState.ELEVATION

        elif self.state == AgentState.ELEVATION:
            print(f"[Agent] Mode ÉLÉVATION (Niveau {self.level}) : préparation du rituel.")
            if self.level == 1 or self.grouper.role == 'LEADER':
                required_stones = self.farmer.requirements.get(self.level, {})
                for stone_name, amount in required_stones.items():
                    for _ in range(amount):
                        self.cmd.set_object(stone_name)
                success = self.cmd.incantation()
                if success:
                    self.level += 1
                    print(f"[Agent] SUCCÈS, passe au Niveau {self.level} !")
                else:
                    print("[Agent] Échec de l'incantation.")
                
            elif self.grouper.role == 'FOLLOWER':
                success = self.cmd.wait_for_incantation_follower()
                if success:
                    self.level += 1
                    print(f"[Agent] FOLLOWER passe au Niveau {self.level} !")
            self.grouper.role = None
            self.state = AgentState.SURVIVAL

        elif self.state == AgentState.FORKING:
            success = self.cmd.fork()
            if success:
                print("[Agent] Un œuf a été pondu.")
                print("[Agent] Lancement d'un nouveau clone en arrière-plan...")
                port = str(self.cmd.network.port)
                machine = self.cmd.network.machine
                subprocess.Popen([sys.executable, "main.py", "-p", port, "-h", machine, "-n", self.team_name])
            self.state = AgentState.SURVIVAL

    def run(self):
        print("[Agent] Démarrage du cycle.")
        while self.is_alive:
            self.update_internal_state()
            self.evaluate_situation()
            self.act_based_on_state()
            time.sleep(0.1)

    def is_food_critical_for_action(self, estimated_tu_cost):
        food = self.inventory.get('food', 0)
        current_tu = food * 126
        minimum_tu_required = (self.food_minimum * 126)
        if current_tu < (estimated_tu_cost + minimum_tu_required):
            return True
        return False
