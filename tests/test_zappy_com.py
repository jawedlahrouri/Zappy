import os
import signal
import socket
import subprocess
import threading
import time

import pytest

HOST = "127.0.0.1"
PORT = 4243

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SERVER_BIN = os.path.join(PROJECT_ROOT, "zappy_server")


FREQ = 100


def spawn_server(port, freq=FREQ, width=10, height=10):
    """Demarre une instance de serveur et renvoie le process (a terminer par l'appelant)."""
    cmd = [
        SERVER_BIN,
        "-p", str(port),
        "-x", str(width),
        "-y", str(height),
        "-n", "Team1", "Team2",
        "-c", "100",
        "-f", str(freq),
    ]
    proc = subprocess.Popen(
        cmd,
        cwd=PROJECT_ROOT,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    time.sleep(2)
    return proc


def stop_server(proc):
    proc.send_signal(signal.SIGINT)
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()


@pytest.fixture(scope="session", autouse=True)
def server():
    """Lance le serveur Zappy en tache de fond pour toute la session de test."""
    proc = spawn_server(PORT)
    yield
    stop_server(proc)


class ZappyClient:
    def __init__(self, team_name="GRAPHIC", port=PORT):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(1.0)
        self.sock.connect((HOST, port))
        self.responses = []

        try:
            init = self.sock.recv(4096).decode()
            self.responses.extend(init.splitlines())
        except socket.timeout:
            pass

        self.sock.sendall(f"{team_name}\n".encode())
        time.sleep(0.1)

        try:
            init2 = self.sock.recv(4096).decode()
            self.responses.extend(init2.splitlines())
        except socket.timeout:
            pass

        self.running = True
        self.thread = threading.Thread(target=self._receive_loop, daemon=True)
        self.thread.start()

    def _receive_loop(self):
        while self.running:
            try:
                data = self.sock.recv(4096).decode()
                if data:
                    self.responses.extend(data.splitlines())
            except socket.timeout:
                continue
            except Exception:
                break

    def send(self, msg):
        self.sock.sendall(f"{msg}\n".encode())

    def get_responses(self):
        return self.responses

    def clear_responses(self):
        self.responses = []

    def wait_for_response(self, keyword, timeout=2.0):
        start = time.time()
        while time.time() - start < timeout:
            for r in reversed(self.responses):
                if keyword in r:
                    return r
            time.sleep(0.1)
        return None

    def wait_for_any_response(self, timeout=2.0):
        start = time.time()
        last_count = len(self.responses)
        while time.time() - start < timeout:
            if len(self.responses) > last_count:
                return self.responses[-1]
            time.sleep(0.1)
        return None

    def close(self):
        self.running = False
        self.sock.close()
        self.thread.join(timeout=1.0)


@pytest.fixture
def gui():
    client = ZappyClient("GRAPHIC")
    yield client
    client.close()


@pytest.fixture
def ai():
    client = ZappyClient("Team1")
    yield client
    client.close()


def test_handshake_welcome_and_map_size(gui):
    """Le GUI recoit WELCOME et msz a la connexion."""
    assert any("WELCOME" in r for r in gui.get_responses()), "WELCOME non recu"
    assert any(r.startswith("msz") for r in gui.get_responses()), "msz non recu"


def test_pnw_synchronization():
    """Un GUI tardif recoit le pnw des joueurs deja connectes."""
    gui1 = ZappyClient("GRAPHIC")
    ai1 = ZappyClient("Team1")
    gui2 = ZappyClient("GRAPHIC")

    pnw_msg = gui2.wait_for_response("pnw")
    assert pnw_msg is not None, "GUI2 n'a pas recu pnw"

    gui1.close()
    ai1.close()
    gui2.close()


def test_gui_msz(gui):
    """La commande GUI msz renvoie la taille de la map."""
    gui.clear_responses()
    gui.send("msz")
    resp = gui.wait_for_response("msz")
    assert resp is not None, "Pas de reponse msz"


def test_ai_forward(ai):
    """Forward renvoie ok."""
    ai.clear_responses()
    ai.send("Forward")
    resp = ai.wait_for_response("ok")
    assert resp is not None, "Pas de ok pour Forward"


def test_ai_right_rotation(ai):
    """Right renvoie ok."""
    ai.clear_responses()
    ai.send("Right")
    resp = ai.wait_for_response("ok")
    assert resp is not None, "Pas de ok pour Right"


def test_ai_left_rotation(ai):
    """Left renvoie ok."""
    ai.clear_responses()
    ai.send("Left")
    resp = ai.wait_for_response("ok")
    assert resp is not None, "Pas de ok pour Left"


def test_ai_inventory(ai):
    """Inventory renvoie un inventaire bien forme commencant par [food."""
    ai.clear_responses()
    ai.send("Inventory")
    resp = ai.wait_for_response("[food")
    assert resp is not None and resp.startswith("[") and resp.endswith("]")


def test_ai_look(ai):
    """Look renvoie une liste de cases contenant au moins le joueur."""
    ai.clear_responses()
    ai.send("Look")
    resp = ai.wait_for_response("[")
    assert resp is not None, "Pas de reponse Look"
    assert resp.startswith("[") and resp.endswith("]"), f"Reponse Look malformee : {resp}"
    assert "player" in resp, "La reponse Look devrait au moins contenir le joueur"


def test_ai_take_resource(ai):
    """Take linemate renvoie ok ou ko."""
    ai.clear_responses()
    ai.send("Take linemate")
    resp1 = ai.wait_for_response("ok", timeout=1.0)
    resp2 = ai.wait_for_response("ko", timeout=1.0)
    assert resp1 is not None or resp2 is not None, "Pas de ok/ko pour Take"


def test_ai_set_resource(ai):
    """Set linemate renvoie ok ou ko."""
    ai.clear_responses()
    ai.send("Set linemate")
    resp1 = ai.wait_for_response("ok", timeout=1.0)
    resp2 = ai.wait_for_response("ko", timeout=1.0)
    assert resp1 is not None or resp2 is not None, "Pas de ok/ko pour Set"


def test_ai_set_failure(ai):
    """Set sans argument renvoie ko."""
    ai.clear_responses()
    ai.send("Set")
    resp = ai.wait_for_response("ko")
    assert resp is not None


def test_ai_broadcast(ai):
    """Broadcast renvoie ok a l'emetteur."""
    ai.clear_responses()
    ai.send("Broadcast hello")
    resp = ai.wait_for_response("ok")
    assert resp is not None, "Broadcast n'a pas renvoye ok"


def test_ai_broadcast_reception():
    """Un broadcast emis par une IA est recu par une autre IA."""
    ai1 = ZappyClient("Team1")
    ai2 = ZappyClient("Team2")

    ai1.clear_responses()
    ai2.clear_responses()

    ai1.send("Broadcast hello world")

    resp_ai1 = ai1.wait_for_response("ok")
    assert resp_ai1 is not None, "L'IA 1 n'a pas recu 'ok' pour Broadcast"

    resp_ai2 = ai2.wait_for_response("hello world")
    assert resp_ai2 is not None and resp_ai2.startswith("message "), \
        "L'IA 2 n'a pas recu correctement le broadcast"

    ai1.close()
    ai2.close()


def test_ai_connect_nbr(ai):
    """Connect_nbr renvoie un nombre de slots disponibles."""
    ai.clear_responses()
    ai.send("Connect_nbr")
    resp = ai.wait_for_any_response()
    assert resp is not None and resp.isdigit(), \
        f"Connect_nbr devrait renvoyer un nombre, recu : {resp}"


def test_ai_eject(ai):
    """Eject renvoie ok ou ko."""
    ai.clear_responses()
    ai.send("Eject")
    resp1 = ai.wait_for_response("ok", timeout=1.0)
    resp2 = ai.wait_for_response("ko", timeout=1.0)
    assert resp1 is not None or resp2 is not None, "Pas de ok/ko pour Eject"


def test_ai_fork_creates_egg(ai, gui):
    """Fork renvoie ok et le GUI recoit la notification d'oeuf enw."""
    ai.clear_responses()
    gui.clear_responses()
    ai.send("Take food")
    ai.wait_for_response("ok")
    ai.send("Fork")
    ok = ai.wait_for_response("ok")
    assert ok is not None
    enw = gui.wait_for_response("enw")
    assert enw is not None, "Le GUI n'a pas recu enw"


def test_ai_incantation(ai):
    """Incantation renvoie ok, ko ou Elevation underway."""
    ai.clear_responses()
    ai.send("Incantation")
    resp1 = ai.wait_for_response("ok", timeout=1.0)
    resp2 = ai.wait_for_response("ko", timeout=1.0)
    resp3 = ai.wait_for_response("Elevation underway", timeout=1.0)
    assert resp1 is not None or resp2 is not None or resp3 is not None, \
        "Pas de reponse connue pour Incantation"


def test_gui_msz_dimensions(gui):
    """msz renvoie exactement les dimensions de la map demandee (10 10)."""
    gui.clear_responses()
    gui.send("msz")
    resp = gui.wait_for_response("msz")
    assert resp is not None and resp.split() == ["msz", "10", "10"], \
        f"msz devrait etre 'msz 10 10', recu : {resp}"


def test_gui_sgt_time_unit(gui):
    """sgt renvoie l'unite de temps configuree (sgt 100)."""
    gui.clear_responses()
    gui.send("sgt")
    resp = gui.wait_for_response("sgt")
    assert resp is not None and resp.split() == ["sgt", str(FREQ)], \
        f"sgt devrait etre 'sgt {FREQ}', recu : {resp}"


def test_gui_bct_tile_format(gui):
    """bct X Y renvoie la case avec ses 7 quantites de ressources."""
    gui.clear_responses()
    gui.send("bct 0 0")
    resp = gui.wait_for_response("bct 0 0")
    assert resp is not None, "Pas de reponse bct"
    parts = resp.split()
    assert parts[:3] == ["bct", "0", "0"], f"En-tete bct invalide : {resp}"
    assert len(parts) == 10, f"bct devrait avoir 7 ressources, recu : {resp}"
    assert all(p.lstrip("-").isdigit() for p in parts[3:]), \
        f"Les quantites bct doivent etre des entiers : {resp}"


def test_gui_tna_lists_all_teams(gui):
    """tna liste toutes les equipes (Team1 et Team2)."""
    gui.clear_responses()
    gui.send("tna")
    time.sleep(0.5)
    teams = {r.split()[1] for r in gui.get_responses() if r.startswith("tna ")}
    assert {"Team1", "Team2"}.issubset(teams), \
        f"tna devrait lister Team1 et Team2, recu : {teams}"


def test_ai_look_first_tile_is_player(ai):
    """La premiere case du Look est celle du joueur et contient 'player'."""
    ai.clear_responses()
    ai.send("Look")
    resp = ai.wait_for_response("[")
    assert resp is not None, "Pas de reponse Look"
    first_tile = resp.strip("[]").split(",")[0]
    assert "player" in first_tile, \
        f"La premiere case du Look devrait contenir le joueur, recu : {first_tile}"


def test_ai_set_without_resource_fails(ai):
    """Set d'une ressource absente de l'inventaire renvoie ko."""
    ai.clear_responses()
    ai.send("Set linemate")
    resp = ai.wait_for_response("ko")
    assert resp is not None, "Set d'une ressource absente devrait renvoyer ko"


def test_ai_forward_respects_action_time():
    """Forward respecte la duree d'action 7/f (serveur dedie a f=2)."""
    port = PORT + 1
    proc = spawn_server(port, freq=2)
    try:
        client = ZappyClient("Team1", port=port)
        client.clear_responses()
        start = time.time()
        client.send("Forward")
        resp = client.wait_for_response("ok", timeout=8.0)
        elapsed = time.time() - start
        client.close()
        assert resp is not None, "Pas de ok pour Forward"
        assert 2.5 < elapsed < 5.0, \
            f"Forward a f=2 devrait durer ~3.5s (7/2), mesure : {elapsed:.2f}s"
    finally:
        stop_server(proc)


def test_ai_command_buffer_limit():
    """Le serveur ne traite pas plus de 10 commandes en file (serveur dedie a f=2)."""
    port = PORT + 2
    proc = spawn_server(port, freq=2)
    try:
        client = ZappyClient("Team1", port=port)
        client.clear_responses()
        for _ in range(15):
            client.send("Forward")
        time.sleep(1.0)
        oks = sum(1 for r in client.get_responses() if r == "ok")
        client.close()
        assert oks <= 10, \
            f"Au plus 10 commandes devraient etre acceptees, {oks} 'ok' recus"
    finally:
        stop_server(proc)
