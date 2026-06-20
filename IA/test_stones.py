#!/usr/bin/env python3

requirements = {
    1: {'linemate': 1},
    2: {'linemate': 1, 'deraumere': 1, 'sibur': 1},
    3: {'linemate': 2, 'deraumere': 0, 'sibur': 1, 'mendiane': 0, 'phiras': 2},
    4: {'linemate': 1, 'deraumere': 1, 'sibur': 2, 'mendiane': 0, 'phiras': 1},
    5: {'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
    6: {'linemate': 1, 'deraumere': 2, 'sibur': 3, 'mendiane': 0, 'phiras': 1},
    7: {'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1},
}

def get_max_level_stones(level):
    total_needed = {}
    max_level = max(requirements.keys()) 
    for l in range(level, max_level + 1):
        needed_for_level = requirements.get(l, {})
        for stone, amount in needed_for_level.items():
            total_needed[stone] = total_needed.get(stone, 0) + amount
    return total_needed

print("Level 3:", get_max_level_stones(3))
print("Level 6:", get_max_level_stones(6))
