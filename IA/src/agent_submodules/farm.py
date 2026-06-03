class Farmer:
    def __init__(self):
        # La liste des prérequis par niveau
        self.requirements = {
            1: {'linemate': 1},
            2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
            4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
            5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
            7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
        }

    def can_elevate(self):
        needed = self.requirements.get(self.level)
        if needed is None:
            return False
        return all(self.inventory.get(item, 0) >= amount for item, amount in needed.items())

    def get_missing_stones(self):
        requirements = {
            1: {'linemate': 1},
            2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
            4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
            5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
            7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
        }
        needed = requirements.get(self.level, {})
        missing = []
        for item, amount in needed.items():
            if self.inventory.get(item, 0) < amount:
                missing.append(item)                
        return missing
