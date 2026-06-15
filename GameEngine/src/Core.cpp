/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#include "Core.hpp"
#include <limits>

Core::Core() : _appState(MENU), _isRunning(false)
{
    initCommands();
}
Core::~Core()
{

}

int Core::run(int ac, char** av)
{
    _isRunning = true;
    setupMenuDefaults(ac, av);

    while (_windowManager.isOpen()) {
        if (_appState == MENU && _windowManager.consumeMenuConnectionEdited()) {
            _network.disconnect();
            _windowManager.resetMenuTeamNames();
        }
        if (_appState == MENU && _windowManager.handleMenuEvents()) {
            if (_network.isConnected())
                _appState = GAME;
            else
                connectFromMenu();
        }
        if (_appState == GAME && _windowManager.handleGameEvents(_world)) {
            _network.disconnect();
            _windowManager.setMenuStatus("Disconnected");
            _appState = MENU;
        }
        if (_appState == GAME)
            sendPendingTileRequest();

        if (_network.isConnected()) {
            std::string command = _network.receiveData();
            if (!command.empty()) {
                parseCommand(command);
            }
        }
 
        _windowManager.clear();

        if (_appState == MENU)
            _windowManager.drawMenu();
        else if (_appState == GAME)
            _windowManager.drawWorld(_world);

        _windowManager.display();
    }

    return 0;
}

void Core::setupMenuDefaults(int ac, char** av)
{
    const std::string port = (ac > 1) ? av[1] : "4242";
    const std::string ip = (ac > 2) ? av[2] : "127.0.0.1";

    _windowManager.setMenuConnectionInfo(ip, port);
}

bool Core::connectFromMenu()
{
    const std::string ip = _windowManager.getMenuIp();
    const std::string portInput = _windowManager.getMenuPort();
    int parsedPort = 0;

    if (ip.empty() || portInput.empty()) {
        _windowManager.setMenuStatus("IP and port are required");
        return false;
    }
    try {
        std::size_t parsedSize = 0;

        parsedPort = std::stoi(portInput, &parsedSize);
        if (parsedSize != portInput.size() || parsedPort <= 0 ||
            parsedPort > std::numeric_limits<unsigned short>::max()) {
            _windowManager.setMenuStatus("Invalid port");
            return false;
        }
    } catch (const std::exception&) {
        _windowManager.setMenuStatus("Invalid port");
        return false;
    }
    _windowManager.setMenuStatus("Connecting...");
    _world = GameWorld();
    _windowManager.resetMenuTeamNames();
    if (!_network.connectToServer(ip, static_cast<unsigned short>(parsedPort))) {
        _windowManager.setMenuStatus("Connection failed");
        return false;
    }
    if (!_network.authenticateAsGraphic()) {
        _network.disconnect();
        _windowManager.setMenuStatus("Authentication failed");
        return false;
    }
    _windowManager.setMenuStatus("Server ready - Press Enter");
    std::cout << "[Core] Moteur démarré. Écoute du serveur..." << std::endl;
    return true;
}

void Core::sendPendingTileRequest()
{
    std::optional<std::pair<int, int>> request = _windowManager.consumeTileRequest();

    if (!request.has_value())
        return;

    const std::string command = "bct " + std::to_string(request->first) +
        " " + std::to_string(request->second) + "\n";

    _network.sendCommand(command);
}

void Core::parseValue(std::stringstream& ss, int& value)
{
    ss >> std::ws;
    if (ss.peek() == '#') {
        char hash = 0;
        ss >> hash;
    }
    ss >> value;
}

void Core::initCommands()
{
    _commands["msz"] = [this](std::stringstream& ss) {
        int width = 0, height = 0;
        parseValue(ss, width);
        parseValue(ss, height);
        _world.setMapSize(width, height);
    };

    _commands["tna"] = [this](std::stringstream& ss) { std::string teamName; ss >> teamName; _windowManager.addMenuTeamName(teamName);};

    _commands["pnw"] = [this](std::stringstream& ss) {
        Player p;
        parseValue(ss, p.id);
        parseValue(ss, p.x);
        parseValue(ss, p.y);
        parseValue(ss, p.orientation);
        parseValue(ss, p.level);
        ss >> p.teamName;
        _windowManager.addMenuTeamName(p.teamName);
        _world.addPlayer(p);
    };

    _commands["ppo"] = [this](std::stringstream& ss) {
        int id = 0, x = 0, y = 0, orientation = 0;
        parseValue(ss, id);
        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, orientation);
        _world.movePlayer(id, x, y, orientation);
    };

    _commands["bct"] = [this](std::stringstream& ss) {
        int x = 0, y = 0, q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0;
        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, q0);
        parseValue(ss, q1);
        parseValue(ss, q2);
        parseValue(ss, q3);
        parseValue(ss, q4);
        parseValue(ss, q5);
        parseValue(ss, q6);
        _world.setTileResources(x, y, _world.resToResources(q0, q1, q2, q3, q4, q5, q6));
    };

    _commands["enw"] = [this](std::stringstream& ss) {
        int eggId = 0, playerId = 0, x = 0, y = 0;
        parseValue(ss, eggId);
        parseValue(ss, playerId);
        parseValue(ss, x);
        parseValue(ss, y);
        std::cout << "[Graphic] enw handler: eggId=" << eggId << " playerId=" << playerId << " x=" << x << " y=" << y << std::endl;
        _world.addEgg(eggId, playerId, x, y);
    };

    _commands["ebo"] = [this](std::stringstream& ss) {
        int eggId = 0;
        parseValue(ss, eggId);
        std::cout << "[Graphic] ebo handler: eggId=" << eggId << std::endl;
        _world.removeEgg(eggId);
    };

    _commands["edi"] = [this](std::stringstream& ss) {
        int eggId = 0;
        parseValue(ss, eggId);
        std::cout << "[Graphic] edi handler: eggId=" << eggId << std::endl;
        _world.removeEgg(eggId);
    };
}

void Core::parseCommand(const std::string& command)
{
    std::string cleanCmd = command;
    if (!cleanCmd.empty() && cleanCmd.back() == '\r') {
        cleanCmd.pop_back();
    }
    std::stringstream ss(cleanCmd);
    std::string cmdType;

    // std::cout << "[Graphic] Recv: " << cleanCmd << std::endl;
    
    ss >> cmdType;

    auto it = _commands.find(cmdType);
    if (it != _commands.end()) {
        it->second(ss);
    } else {
        std::cout << "[Graphic] No handler found for: " << cmdType << std::endl;
    }
}
