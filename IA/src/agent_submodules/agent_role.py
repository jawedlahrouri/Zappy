#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentRole")

class AgentRole(Enum): # enum pour representer les différents roles que les drone/clone adopterons
    QUEEN = 1
    GUARD = 2
    PROTECTOR = 3
    PEASANT = 4