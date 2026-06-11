/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#include "Core.hpp"

Core::Core() : _appState(MENU), _isRunning(false)
{

}
Core::~Core()
{

}

int Core::run(int ac, char** av)
{
    unsigned short port = (ac > 1) ? static_cast<unsigned short>(std::stoi(av[1])) : 4242;

    if (!_network.connectToServer("127.0.0.1", port))
        return 84;

    if (!_network.authenticateAsGraphic())
        return 84;

    _isRunning = true;
    std::cout << "[Core] Moteur démarré. Écoute du serveur..." << std::endl;

    while (_windowManager.isOpen()) {
        if (_appState == MENU && _windowManager.handleMenuEvents())
            _appState = GAME;
        if (_appState == GAME && _windowManager.handleGameEvents(_world))
            _appState = MENU;
        if (_appState == GAME)
            sendPendingTileRequest();

        std::string command = _network.receiveData();
        if (!command.empty()) {
            parseCommand(command);
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
        _world.addPlayer(p);
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
