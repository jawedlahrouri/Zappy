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

void Core::parseCommand(const std::string& command)
{
    std::stringstream ss(command);
    std::string cmdType;

    std::cout << command << std::endl;
    
    ss >> cmdType;

    if (cmdType == "msz") {
        int width = 0, height = 0;
        ss >> width >> height;
        _world.setMapSize(width, height);
    }
    else if (cmdType == "pnw") {
        Player p;
        ss >> std::ws;
        if (ss.peek() == '#') {
            char hash = 0;
            ss >> hash;
        }
        ss >> p.id >> p.x >> p.y >> p.orientation >> p.level >> p.teamName;
        _windowManager.addMenuTeamName(p.teamName);
        _world.addPlayer(p);
    }
    else if (cmdType == "tna") {
        std::string teamName;

        ss >> teamName;
        _windowManager.addMenuTeamName(teamName);
    }
    else if (cmdType == "ppo") {
        int id = 0, x = 0, y = 0, orientation = 0;
        ss >> std::ws;
        if (ss.peek() == '#') {
            char hash = 0;
            ss >> hash;
        }
        ss >> id >> x >> y >> orientation;

        _world.movePlayer(id, x, y, orientation);
    }
    else if (cmdType == "bct") {
        int x = 0, y = 0, q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0;
        ss >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6;

        _world.setTileResources(x, y, _world.resToResources(q0, q1, q2, q3, q4, q5, q6));
    }
    //ajouter les autres commandes
}
