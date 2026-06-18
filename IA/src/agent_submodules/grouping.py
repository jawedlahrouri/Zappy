#!/usr/bin/python3
from src.logger import ZappyLogger

logger = ZappyLogger.get_logger("Grouper")

class Grouper:
    def __init__(self, command_interface, stealth_comms, agent):
        self.cmd = command_interface # Interface de commande pour interagir avec le serveur.
        self.comms = stealth_comms # Module de communication furtive pour encoder/décoder les messages de regroupement.
        self.agent = agent # Référence à l'agent auquel appartient ce groupe.
        self.role = None # Rôle de l'agent dans le regroupement : 'LEADER' ou 'FOLLOWER'.
        self.current_k = None # La direction du son entendu pour le regroupement, utilisée par les FOLLOWER pour se déplacer vers le LEADER.
        self.player_reqs = {1: 1, 2: 2, 3: 2, 4: 4, 5: 4, 6: 6, 7: 6} # Les exigences de joueurs pour chaque niveau d'incantation, utilisées pour déterminer quand le groupe est complet.

    def _move_towards_sound(self, k):
        """Déplace l'agent vers la direction d'où provient un son, en fonction de la valeur de k reçue.\n
        Params:
            -k (int): La direction du son entendu, de 0 à 8, où 0 signifie que le son vient de la même case et les autres valeurs indiquent des directions spécifiques autour de l'agent.
        Ne retourne rien."""

        if k == 1:
            self.cmd.send_command("Forward")
        elif k in [2, 3]:
            self.cmd.send_command("Left")
            self.cmd.send_command("Forward")
        elif k in [4, 5]:
            self.cmd.send_command("Left")
            self.cmd.send_command("Left")
            self.cmd.send_command("Forward")
        elif k in [6, 7]:
            self.cmd.send_command("Right")
            self.cmd.send_command("Forward")
        elif k == 8:
            self.cmd.send_command("Right")
            self.cmd.send_command("Forward")

    def analyze_messages(self, level, my_id):
        """Analyse les messages de regroupement reçus pour déterminer si l'agent doit être LEADER ou FOLLOWER\n
        Params:
            -level (int): Le niveau actuel de l'agent, utilisé pour filtrer les messages de regroupement pertinents.
            -my_id (int): L'ID unique de l'agent, utilisé pour les tie-breakers dans la détermination des rôles.
        Retourne:
            -tuple (k, type) ou (None, None) si rien de pertinent."""
        
        sorted_senders = sorted(self.agent.message_stack.keys(), reverse=True)
        for sender_id in sorted_senders:
            messages = self.agent.message_stack[sender_id]
            for msg_dict in messages:
                for k, (command, params_list) in msg_dict.items():
                    if sender_id == 9999 and command == "grouping":
                        logger.info(f"Appel de la Reine reçu de la direction {k}. En route !")
                        self.current_k = k
                        return k, "ROYAL_GROUPING"
                    if command == "needmate":
                        sender_level = int(params_list[0]) if len(params_list) > 0 else None
                        if sender_level == self.agent.level:
                            self.current_k = k
                            if sender_id > self.agent.agent_id:
                                logger.info(f"Allié (ID:{sender_id}) de même niveau cherche partenaire. Je deviens FOLLOWER.")
                                return k, "FOLLOWER"
                            elif sender_id < self.agent.agent_id:
                                logger.info(f"Allié (ID:{sender_id}) de même niveau cherche partenaire. Je deviens LEADER.")
                                return k, "LEADER"
        return None, None
    
    def process_grouping(self, level, my_id, vision):
        """Procède au regroupement en fonction du niveau, de l'ID et de la vision de l'agent.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -my_id (int): L'ID unique de l'agent.
            -vision (list): La vision actuelle de l'agent.
        Retourne:
            -bool: True si le regroupement est réussi, False sinon."""
        call_k, grouping_type = self.analyze_messages(level, my_id)
        
        # Gestion du Décret Royal (prioritaire)
        if grouping_type == 'ROYAL_GROUPING':
            return self.act_royal_grouping(level, my_id, vision, call_k)
            
        # Mise à jour du rôle selon les messages reçus
        if grouping_type in ['LEADER', 'FOLLOWER']:
            self.role = grouping_type
        # Si on n'a toujours pas de rôle, on devient LEADER par défaut (comme dans l'ancienne version)
        elif self.role is None:
            self.role = 'LEADER'

        # Agir selon le rôle mémorisé
        if self.role == 'LEADER':
            return self.act_leader(level, my_id, vision)
        elif self.role == 'FOLLOWER':
            return self.act_follower(level, my_id, vision)
            
        return False

    def act_grouping_protector(self):
        queen_k = self.agent.k_queen
        protector_index = self.agent.agent_id % 4  # Calculer l'index du protecteur basé sur son ID
        
        if queen_k is not None:
            if not hasattr(self.agent, 'is_deployed'):
                self.agent.is_deployed = False

            if not self.agent.is_deployed:
                if queen_k == 0:
                    # Déjà sur la case de la reine, se positionner sur une case adjacente
                    if protector_index == 0:
                        self.cmd.send_command("Forward")
                    elif protector_index == 1:
                        self.cmd.send_command("Right")
                        self.cmd.send_command("Forward")
                    elif protector_index == 2:
                        self.cmd.send_command("Right")
                        self.cmd.send_command("Right")
                        self.cmd.send_command("Forward")
                    elif protector_index == 3:
                        self.cmd.send_command("Left")
                        self.cmd.send_command("Forward")

                    # Faire face vers l'extérieur
                    self.cmd.send_command("Right")
                    self.cmd.send_command("Right")
                    self.agent.is_deployed = True
                else:
                    # Pas encore sur la case de la reine, s'en rapprocher
                    self._move_towards_sound(queen_k)

    def act_royal_grouping(self, level, my_id, vision, call_k):
        """Effectue les actions de regroupement royal.\n
        Params:
            -level (int): Le niveau actuel de l'agent.
            -my_id (int): L'ID unique de l'agent.
            -vision (list): La vision actuelle de l'agent.
            -call_k (int): La direction de l'appel de regroupement royal.
        Retourne:
            -bool: True si le regroupement est réussi, False sinon."""

        from src.agent_submodules.agent_role import AgentRole
        from src.agent_submodules.agent_state import AgentState

        if self.agent.role == AgentRole.QUEEN:
            logger.info(f"Reine : J'ai lancé un appel de regroupement royal. Tous les alliés doivent se rassembler.")
            self.cmd.send_command("Broadcast " + self.comms.encode(f"{my_id}-{level}", "grouping"))
            return False
        elif self.agent.role == AgentRole.GUARD:
            if call_k == 0:
                logger.info(f"Garde : Je suis arrivé à la case de la Reine. Prêt pour l'élévation.")
                self.agent.comportment.act_elevating()
                return True
            else:
                logger.info(f"Garde : J'ai entendu l'appel de regroupement royal. Je me dirige vers la Reine.")
                self._move_towards_sound(call_k)
                return False
        elif self.agent.role == AgentRole.PROTECTOR:
            if call_k == 0:
                logger.info(f"Protecteur : Je suis arrivé à la case de la Reine. Je vais l'aider à se protéger.")
                self.agent.comportment.act_pushing()
                return True
            else:
                logger.info(f"Protecteur : J'ai entendu l'appel de regroupement royal. Je me dirige vers la Reine.")
                self._move_towards_sound(call_k)
                return False
        elif self.agent.role == AgentRole.PEASANT:
            self.agent.state = AgentState.FEEDER
            if call_k == 0:
                logger.info(f"Paysan : Je suis arrivé à la case de la Reine. Je vais l'aider à se nourrir.")
                self.agent.comportment.act_feeder()
                return True
            else:
                logger.info(f"Paysan : J'ai entendu l'appel de regroupement royal. Je me dirige vers la Reine.")
                self._move_towards_sound(call_k)
                return False
        return False

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
        from src.agent_submodules.agent_state import AgentState
        if self.current_k == 0:
            players_on_tile = vision[0].count('player') if len(vision) > 0 else 0
            needed = self.player_reqs.get(level, 1)
            if players_on_tile >= needed:
                logger.info(f"agent : Équipe au complet ({players_on_tile}/{needed}). Prêt pour le rituel en tant que FOLLOWER !")
                self.role = 'FOLLOWER'
                self.agent.state = AgentState.ELEVATION
                self.current_k = None
                return True
            else:
                logger.info(f"agent : Sur la case, attente des autres alliés ({players_on_tile}/{needed})...")
                self.current_k = None
                return False
        else:
            logger.info(f"agent : En route vers K={self.current_k} pour rejoindre le LEADER.")
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
        from src.agent_submodules.agent_state import AgentState
        if players_on_tile >= needed:
            logger.info(f"agent : Équipe au complet ({players_on_tile}/{needed}). Prêt pour le rituel en tant que LEADER !")
            self.role = 'LEADER'
            self.agent.state = AgentState.ELEVATION
            return True
        logger.info(f"agent : Attente alliés. Broadcast...")
        secret_msg = self.comms.encode(f"{my_id}-{level}", "needmate")
        self.cmd.send_command(f"Broadcast {secret_msg}")
        return False
