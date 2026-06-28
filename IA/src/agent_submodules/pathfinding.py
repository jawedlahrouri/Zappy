#!/usr/bin/python3
import math
import random
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("Pathfinder")

class Pathfinder:
    def __init__(self, command_interface):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.explore_step = 0 # Variable pour suivre les étapes d'exploration et de scan, permettant de varier les mouvements de l'agent pour une exploration plus efficace.

    def _score_tile(self, index, tile_contents, target_items):
        """Calcule un score pour une case en fonction des éléments recherchés.\n
        Params:
            -index (int): L'index de la case.
            -tile_contents (list): Les éléments présents sur la case.
            -target_items (list): Les éléments à rechercher.
        Retourne:
            -int: Le score de la case."""

        score = 0
        items_found = 0
        
        if tile_contents.count('player') >= 6:
            stones = ['linemate', 'deraumere', 'sibur', 'mendiane', 'phiras', 'thystame']
            if any(item in stones for item in target_items):
                return -999
                
        for item in target_items:
            count = tile_contents.count(item)
            items_found += count
            score += count * 10 
        if items_found == 0:
            return -999
        row = int(math.sqrt(index))
        middle_of_row = row * row + row
        forward_cost = row 
        turn_cost = 1 if index != 0 and index != middle_of_row else 0
        distance_cost = forward_cost + turn_cost
        score -= distance_cost * 2 
        return score

    def go_to_tile(self, target_index):
        """Déplace l'agent vers une case cible en fonction de son index dans la vision.\n
        Params:
            -target_index (int): L'index de la case cible dans la vision.
        Ne retourne rien."""

        if target_index == 0:
            return
        row = int(math.sqrt(target_index))
        for _ in range(row):
            self.cmd.send_command("Forward")
        middle_of_row = row * row + row
        shift = target_index - middle_of_row
        if shift < 0:
            self.cmd.send_command("Left")
            for _ in range(abs(shift)):
                self.cmd.send_command("Forward")
        elif shift > 0:
            self.cmd.send_command("Right")
            for _ in range(shift):
                self.cmd.send_command("Forward")

    def search_and_take_items(self, target_items, vision):
        """Cherche les cases contenant les éléments ciblés et se déplace vers la meilleure case pour les ramasser.\n
        Params:
            -target_items (list): Les éléments à rechercher.
            -vision (list): La vision actuelle de l'agent, une liste de cases avec leur contenu.
        Retourne:
            -bool: True si une case cible a été trouvée et traitée, False sinon."""

        best_index = -1
        best_score = -999
        for index, tile_contents in enumerate(vision):
            score = self._score_tile(index, tile_contents, target_items)
            if score > best_score:
                best_score = score
                best_index = index
        if best_index != -1:
            logger.info(f"Agent se déplace vers la case {best_index} pour ramasser des items. Score de la case: {best_score}. Items ciblés: {target_items}. Contenu de la case: {vision[best_index]}")  
            self.explore_step = 0
            self.go_to_tile(best_index)
            items_on_tile = vision[best_index]
            for item in target_items:
                count = items_on_tile.count(item)
                for _ in range(count):
                    logger.info(f"Agent ramasse {item} sur la case {best_index}.")
                    self.cmd.send_command(f"Take {item}")
            return True
        return False

    def explore(self, level):
        """Explore la zone autour de l'agent en fonction de son niveau.\n
        Params:
            -level (int): Le niveau de l'agent.
        Ne retourne rien."""

        if level <= 2:
            if self.explore_step < 3:
                logger.info(f"Agent explore en avançant droit devant. Étape {self.explore_step + 1}/3.")
                self.cmd.send_command("Forward")
                self.explore_step += 1
            else:
                logger.info(f"Agent explore en virant pour quadriller la zone. Étape {self.explore_step + 1}/3.")
                action = random.choice(["Right", "Left"])
                self.cmd.send_command(action)
                self.explore_step = 0 
        else:
            if self.explore_step < 3:
                logger.info(f"Agent explore en scannant à droite. Étape {self.explore_step + 1}/3.")
                self.cmd.send_command("Right")
                self.explore_step += 1
            else:
                logger.info(f"Agent explore en avançant après scan. Étape {self.explore_step + 1}/3.")
                self.cmd.send_command("Forward")
                self.explore_step = 0
