import random

class Grouper:
    def __init__(self, command_interface, stealth_comms):
        self.cmd = command_interface
        self.comms = stealth_comms
        self.role = None
        self.current_k = None
        self.player_reqs = {1: 1, 2: 2, 3: 2, 4: 4, 5: 4, 6: 6, 7: 6}

    def _move_towards_sound(self, k):
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

    def analyze_calls(self, level, my_id, unread_messages):
        heard_k = None
        for msg in unread_messages:
            try:
                clean_msg = msg.replace("message ", "")
                parts = clean_msg.split(",", 1)
                k = int(parts[0].strip())
                raw_text = parts[1].strip()
                decoded = self.comms.decode(raw_text)
                if decoded and decoded['command'] == 'need_mate':
                    param_parts = decoded['param'].split('-')
                    msg_level = int(param_parts[0].replace('lv', ''))
                    msg_id = int(param_parts[1])
                    if msg_level == level:
                        if self.role != 'LEADER' or msg_id > my_id:
                            heard_k = k
                            print(f"[Grouper] Tie-Breaker PERDU face à {msg_id}. On capitule.")
                        else:
                            print(f"[Grouper] Tie-Breaker GAGNÉ face à {msg_id}. On l'ignore.")
            except Exception as e:
                continue
        return heard_k

    def process_grouping(self, level, my_id, vision):
        if self.current_k is not None:
            self.role = 'FOLLOWER'
        elif self.role is None:
            self.role = 'LEADER'
        if self.role == 'FOLLOWER':
            if self.current_k == 0:
                print("[Grouper] FOLLOWER : Arrivé sur la case du Leader")
                self.current_k = None
                return True
            else:
                print(f"[Grouper] FOLLOWER : En route vers K={self.current_k}")
                self._move_towards_sound(self.current_k)
                self.current_k = None
                return False

        elif self.role == 'LEADER':
            players_on_tile = vision[0].count('player') if len(vision) > 0 else 0
            needed = self.player_reqs.get(level, 1)
            if players_on_tile >= needed:
                print(f"[Grouper] LEADER : Équipe au complet ({players_on_tile}/{needed}).")
                return True
            print(f"[Grouper] LEADER (ID: {my_id}) : Attente alliés. Broadcast...")
            secret_msg = self.comms.encode(f"lv{level}-{my_id}", "needmate")
            self.cmd.broadcast_text(secret_msg)
            return False
