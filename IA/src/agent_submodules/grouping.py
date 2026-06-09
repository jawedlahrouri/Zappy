#!/usr/bin/python3
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("Grouper")

class Grouper:
    def __init__(self, command_interface, stealth_comms):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.comms = stealth_comms # Module de communication furtive pour encoder/décoder les messages de regroupement.
        self.role = None # Rôle de l'agent dans le regroupement : 'LEADER' ou 'FOLLOWER'.
        self.current_k = None # La direction du son entendu pour le regroupement, utilisée par les FOLLOWER pour se déplacer vers le LEADER.
        self.player_reqs = {1: 1, 2: 2, 3: 2, 4: 4, 5: 4, 6: 6, 7: 6} # Les exigences de joueurs pour chaque niveau d'incantation, utilisées pour déterminer quand le groupe est complet.

    def _move_towards_sound(self, k):
        """Déplace l'agent vers la direction d'où provient un son, en fonction de la valeur de k reçue.\n
        Params:
            -k (int): La direction du son entendu, de 0 à 8, où 0 signifie que le son vient de la même case et les autres valeurs indiquent des directions spécifiques autour de l'agent.
        Ne retourne rien."""

        if k == 1:
            self.cmd.forward()
        elif k in [2, 3]:
            self.cmd.left()
            self.cmd.forward()
        elif k in [4, 5]:
            self.cmd.left()
            self.cmd.left()
            self.cmd.forward()
        elif k in [6, 7]:
            self.cmd.right()
            self.cmd.forward()
        elif k == 8:
            self.cmd.right()
            self.cmd.forward()

    def analyze_calls(self, level, my_id, decoded_messages):
        """Analyse les messages de regroupement reçus pour déterminer si l'agent doit être LEADER ou FOLLOWER\n
        Params:
            -level (int): Le niveau actuel de l'agent, utilisé pour filtrer les messages de regroupement pertinents.
            -my_id (int): L'ID unique de l'agent, utilisé pour les tie-breakers dans la détermination des rôles.
            -decoded_messages (list): La liste des messages décodés reçus du serveur, contenant les commandes de regroupement des alliés.
        Retourne:
            -int or None: La direction du son entendu, ou None si aucun son n'a été entendu."""
        
        heard_k = None
        for msg_obj in decoded_messages:
            k = msg_obj['k']
            decoded = msg_obj['data']
            if decoded['command'] == 'needmate':
                param_parts = decoded['param'].split('-')
                msg_level = int(param_parts[0].replace('lv', ''))
                msg_id = int(param_parts[1])
                if msg_level == level:
                    if self.role != 'LEADER' or msg_id > my_id:
                        heard_k = k
                        logger.info(f"Agent entend un appel de regroupement pour le niveau {level} de la part de l'agent {msg_id}. Direction du son (k): {k}. Rôle actuel: {self.role}. Décision: Devenir FOLLOWER et se déplacer vers le LEADER.")
                        print(f"[Grouper] Tie-Breaker PERDU face à {msg_id}. On capitule.")
                    else:
                        logger.info(f"Agent entend un appel de regroupement pour le niveau {level} de la part de l'agent {msg_id}. Direction du son (k): {k}. Rôle actuel: {self.role}. Décision: Devenir LEADER, il ne bouge plus.")
                        print(f"[Grouper] Tie-Breaker GAGNÉ face à {msg_id}. On l'ignore.")
        return heard_k

    def process_grouping(self, level, my_id, vision):
        """Procède au regroupement en fonction du niveau, de l'ID et de la vision de l'agent.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -my_id (int): L'ID unique de l'agent.
            -vision (list): La vision actuelle de l'agent.
        Retourne:
            -bool: True si le regroupement est réussi, False sinon."""

        if self.current_k is not None:
            self.role = 'FOLLOWER'
        elif self.role is None:
            self.role = 'LEADER'
        if self.role == 'FOLLOWER':
            return self.act_follower(level, my_id, vision)
        elif self.role == 'LEADER':
            return self.act_leader(level, my_id, vision)
        
    def act_follower(self, level, my_id, vision):
        """Effectue les actions d'un agent suiveur en fonction du niveau, de l'ID et de la vision.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -my_id (int): L'ID unique de l'agent.
            -vision (list): La vision actuelle de l'agent.
        Retourne:
            -bool: True si l'action est réussie, False sinon."""
    
        if self.current_k is None:
            return False
        if self.current_k == 0:
            players_on_tile = vision[0].count('player') if len(vision) > 0 else 0
            needed = self.player_reqs.get(level, 1)
            if players_on_tile >= needed:
                logger.info(f"agent {self.agent_id} : Équipe au complet ({players_on_tile}/{needed}). Ils sont prets pour le rituel !")
                print(f"[Grouper] FOLLOWER : Équipe au complet ({players_on_tile}/{needed}). Prêt pour le rituel !")
                self.current_k = None
                return True
            else:
                logger.info(f"agent {self.agent_id} : Sur la case, attente des autres alliés ({players_on_tile}/{needed})...")
                print(f"[Grouper] FOLLOWER : Sur la case, attente des autres alliés ({players_on_tile}/{needed})...")
                self.current_k = None
                return False
        else:
            logger.info(f"agent {self.agent_id} : En route vers K={self.current_k} pour rejoindre le LEADER.")
            print(f"[Grouper] FOLLOWER : En route vers K={self.current_k}")
            self._move_towards_sound(self.current_k)
            self.current_k = None
            return False

    def act_leader(self, level, my_id, vision):
        """Effectue les actions d'un agent leader en fonction du niveau, de l'ID et de la vision.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -my_id (int): L'ID unique de l'agent.
            -vision (list): La vision actuelle de l'agent.
        Retourne:
            -bool: True si l'action est réussie, False sinon."""

        players_on_tile = vision[0].count('player') if len(vision) > 0 else 0
        needed = self.player_reqs.get(level, 1)
        if players_on_tile >= needed:
            logger.info(f"agent {self.agent_id} : Équipe au complet ({players_on_tile}/{needed}). Ils sont prets pour le rituel !")
            print(f"[Grouper] LEADER : Équipe au complet ({players_on_tile}/{needed}).")
            return True
        logger.info(f"agent {self.agent_id} : Attente alliés. Broadcast...")
        print(f"[Grouper] LEADER (ID: {my_id}) : Attente alliés. Broadcast...")
        secret_msg = self.comms.encode(f"lv{level}-{my_id}", "needmate")
        self.cmd.broadcast_text(secret_msg)
        return False
