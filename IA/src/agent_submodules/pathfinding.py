import math
import random

class Pathfinder:
    def __init__(self, command_interface):
        self.cmd = command_interface
        self.explore_step = 0

    def _score_tile(self, index, tile_contents, target_items):
        score = 0
        items_found = 0
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
        if target_index == 0:
            return
        row = int(math.sqrt(target_index))
        for _ in range(row):
            self.cmd.forward()
        middle_of_row = row * row + row
        shift = target_index - middle_of_row
        if shift < 0:
            self.cmd.left()
            for _ in range(abs(shift)):
                self.cmd.forward()
        elif shift > 0:
            self.cmd.right()
            for _ in range(shift):
                self.cmd.forward()

    def search_and_take_items(self, target_items, vision):
        best_index = -1
        best_score = -999
        for index, tile_contents in enumerate(vision):
            score = self._score_tile(index, tile_contents, target_items)
            if score > best_score:
                best_score = score
                best_index = index
        if best_index != -1:
            print(f"[Agent] Cible optimale: case {best_index} (Score: {best_score}). Déplacement...")
            self.explore_step = 0
            self.go_to_tile(best_index)
            items_on_tile = vision[best_index]
            for item in target_items:
                count = items_on_tile.count(item)
                for _ in range(count):
                    print(f"[Agent] Ramassage multiple : {item}")
                    self.cmd.take_object(item)
            return True
        return False

    def explore(self, level):
        if level <= 2:
            if self.explore_step < 3:
                print(f"[Agent Lvl {level}] Exploration : On trace tout droit.")
                self.cmd.forward()
                self.explore_step += 1
            else:
                print(f"[Agent Lvl {level}] Exploration : Virage pour quadriller la zone.")
                action = random.choice([self.cmd.right, self.cmd.left])
                action()
                self.explore_step = 0 
        else:
            if self.explore_step < 3:
                print(f"[Agent Lvl {level}] Exploration : Scan rotation droite ({self.explore_step + 1}/3)")
                self.cmd.right()
                self.explore_step += 1
            else:
                print(f"[Agent Lvl {level}] Exploration : Scan 360° terminé. On avance !")
                self.cmd.forward()
                self.explore_step = 0
