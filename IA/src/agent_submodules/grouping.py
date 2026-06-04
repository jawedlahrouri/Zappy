import random

class Grouper:
    def __init__(self, command_interface, stealth_comms):
        self.cmd = command_interface
        self.comms = stealth_comms
        self.role = None
        self.target_level = None

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
            
    def listen_and_group(self, level):

        self.target_level = level
        unread_messages = self.cmd.get_unread_broadcasts()
        valid_call_k = None
        for msg in unread_messages:
            try:
                parts = msg.replace("message ", "").split(", ", 1)
                k = int(parts[0])
                raw_text = parts[1]
                decoded = self.comms.decode(raw_text)
                if decoded and decoded['command'] == 'need_mate' and decoded['param'] == f"lv{level}":
                    valid_call_k = k
                    break
            except:
                continue
        if valid_call_k is not None:
            self.role = 'FOLLOWER'
            if valid_call_k == 0:
                print(f"[Grouper] Follower arrivé sur la case du Leader !")
                return True
            print(f"[Grouper] Follower en route vers K={valid_call_k}")
            self._move_towards_sound(valid_call_k)
            return False
        self.role = 'LEADER'
        print(f"[Grouper] Leader s'installe et lance un appel radio...")
        secret_msg = self.comms.encode(f"lv{level}", "need_mate")
        self.cmd.broadcast_text(secret_msg)
        return False
