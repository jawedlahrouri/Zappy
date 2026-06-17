import logging
import sys
import os

class ZappyLogger:
    _initialized = False

    def setup(cls, team_name: str, debug_mode: bool = False):
        """Configure le système de logging pour l'agent Zappy. Crée un logger avec des handlers pour la console et les fichiers, et formate les messages de log.\n
        Params:
            -team_name (str): Le nom de l'équipe.
            -debug_mode (bool): Si True, active le mode debug.
        Retourne None."""

        if cls._initialized:
            return
        logger = logging.getLogger("ZappyAI")
        logger.setLevel(logging.DEBUG if debug_mode else logging.INFO)
        if logger.hasHandlers():
            logger.handlers.clear()
        formatter = logging.Formatter(
            fmt='[%(asctime)s] [PID:%(process)d] [%(levelname)s] [%(name)s] %(message)s',
            datefmt='%H:%M:%S'
        )
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(formatter)
        logger.addHandler(console_handler)
        if not os.path.exists("logs"):
            os.makedirs("logs")
        file_handler = logging.FileHandler(f"logs/{team_name}_logs.txt")
        file_handler.setFormatter(formatter)
        logger.addHandler(file_handler)
        cls._initialized = True

    def get_logger(module_name):
        """Récupère un logger spécifique à un module de l'agent Zappy.\n
        Params:
            -module_name (str): Le nom du module pour lequel on veut le logger.
        Retourne un logger configuré pour le module spécifié."""

        return logging.getLogger(f"ZappyAI.{module_name}")