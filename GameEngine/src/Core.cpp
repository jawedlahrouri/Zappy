/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#include "Core.hpp"

Core::Core() : _isRunning(false) 
{

}
Core::~Core()
{

}

int Core::run(int ac, char** av) {
    unsigned short port = (ac > 1) ? static_cast<unsigned short>(std::stoi(av[1])) : 4242;

    if (!_network.connectToServer("127.0.0.1", port)) {
        return 84;
    }

    if (!_network.authenticateAsGraphic()) {
        return 84;
    }

    _isRunning = true;
    std::cout << "[Core] Moteur démarré. Écoute du serveur..." << std::endl;

    while (_isRunning) {
        std::string command = _network.receiveData();

        if (!command.empty()) {
            parseCommand(command);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}

void Core::parseCommand(const std::string& command) {
    std::stringstream ss(command);
    std::string cmdType;
    
    ss >> cmdType;

    if (cmdType == "msz") {
        int width = 0, height = 0;
        ss >> width >> height;
        _world.setMapSize(width, height);
    }
    else if (cmdType == "pnw") {
        Player p;
        ss >> p.id >> p.x >> p.y >> p.orientation >> p.level >> p.teamName;
        _world.addPlayer(p);
    }
    else if (cmdType == "ppo") {
        int id = 0, x = 0, y = 0, orientation = 0;
        ss >> id >> x >> y >> orientation;

        _world.movePlayer(id, x, y, orientation);
    }
    //ajouter les autres commandes
}