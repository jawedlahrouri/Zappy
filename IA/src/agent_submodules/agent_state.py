#!/usr/bin/python3
from enum import Enum
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("AgentState")

class AgentState(Enum): # Enum pour représenter les différents états de comportement de l'agent.
    SURVIVAL = 1
    FARMING = 2
    GROUPING = 3
    ELEVATION = 4
    BEGINNER = 5
    LEADER = 6
    FOLLOWER = 7
    FEEDER = 8
    PUSHING = 9

class StateComportment:
    def __init__(self, agent):
        self.agent = agent # Référence à l'agent principal pour accéder à ses modules et données.

    def act_survival(self):
        """Effectue les actions de survie de base, comme chercher de la nourriture pour éviter la mort par faim.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour survivre."""

    def act_farming(self):
        """Effectue les actions de farming pour collecter les ressources nécessaires à l'incantation.\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour farming."""

    def act_elevating(self):
        """Effectue les actions nécessaires pour tenter une incantation, en fonction du rôle de l'agent dans le regroupement (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour l'incantation."""

    def act_grouping(self):
        """Effectue les actions de regroupement pour rassembler les agents nécessaires à l'incantation, en fonction du rôle de l'agent (LEADER ou FOLLOWER).\n
        Params:
            -Aucun paramètre.
        Retourne:
            -AgentState: L'état dans lequel l'agent doit rester ou passer après avoir agi pour le regroupement."""

    def act_beginer(self):
        """Effectue les actions attendus de la part d'un bot qui vient d'arriver.
        Params:
            - Aucun parametre.
        Retourne:
            - AgentState: l'etat dans lequel l'agent doit etre.
        """
    
    def act_pushing(self):
        """Eject
        Params:
            - Aucun parametres
        Retourne:
            - Ne return rien
        """