#!/usr/bin/python3
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("Farmer")

class Farmer:
    def __init__(self):
        self.requirements = {
            1: {'linemate': 1},
            2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
            4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
            5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
            7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
        } # Les exigences de ressources pour chaque niveau d'incantation.

    def can_elevate(self, level, inventory):
        """Vérifie si l'agent a les ressources nécessaires pour s'élever au niveau suivant.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -inventory (dict): L'inventaire actuel de l'agent, avec les quantités de chaque ressource.
        Retourne:
            -bool: True si l'agent peut s'élever, False sinon."""

        needed = self.requirements.get(level)
        if needed is None:
            return False
        return all(inventory.get(item, 0) >= amount for item, amount in needed.items())

    def get_missing_stones(self, level, inventory):
        """Retourne la liste des ressources manquantes pour que l'agent puisse s'élever au niveau suivant.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -inventory (dict): L'inventaire actuel de l'agent, avec les quantités de chaque ressource.
        Retourne:
            -list: Une liste des ressources manquantes, vide si aucune ressource n'est manquante."""

        needed = self.requirements.get(level, {})
        missing = []
        for item, amount in needed.items():
            if inventory.get(item, 0) < amount:
                missing.append(item)                
        return missing
    
    def get_max_level_stones(self, level):
        """Retourne la somme des pierres nécessaires pour atteindre le niveau maximum depuis le niveau donné.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
        Retourne:
            -dict: Un dictionnaire contenant le total de chaque pierre nécessaire pour atteindre le niveau 8 (ex: {'linemate': 7, ...})."""

        total_needed = {}
        max_level = max(self.requirements.keys()) 
        for l in range(level, max_level + 1):
            needed_for_level = self.requirements.get(l, {})
            for stone, amount in needed_for_level.items():
                total_needed[stone] = total_needed.get(stone, 0) + amount
        for stone in total_needed:
            total_needed[stone] += 3  # Marge de sécurité minimale
        return total_needed

    def get_missing_stones_for_max_level(self, level, global_inventory):
        """Retourne la liste des pierres manquantes pour atteindre le niveau maximum d'incantation depuis le niveau donné.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -global_inventory (dict): L'inventaire global de l'équipe, avec les quantités de chaque ressource.
        Retourne:
            -list: La liste des noms des pierres manquantes pour le niveau max, vide si aucune pierre n'est manquante."""
        
        total_needed = self.get_max_level_stones(level)
        missing = []
        
        for stone, required_amount in total_needed.items():
            current_amount = global_inventory.get(stone, 0)
            if current_amount < required_amount:
                missing.append(stone)
        return missing
