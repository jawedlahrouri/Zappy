/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#include "Core.hpp"
#include <algorithm>
#include <vector>
#include <limits>

static bool isGraphicPathOption(const std::string& arg)
{
    return arg == "-g" || arg == "--graphic" || arg == "--graphic-lib";
}

static std::vector<int> parseTrailingPlayerIds(std::stringstream& ss)
{
    std::vector<int> playerIds;

    while (true) {
        ss >> std::ws;
        if (ss.eof())
            break;
        if (ss.peek() == '#')
            ss.get();
        int playerId = 0;

        if (!(ss >> playerId))
            break;
        playerIds.push_back(playerId);
    }
    return playerIds;
}

Core::Core() : _appState(MENU), _isRunning(false), _menuReady(false),
    _hasMenuTeams(false), _expectedInitialTiles(0)
{
    initCommands();
}
Core::~Core()
{

}

int Core::run(int ac, char** av)
{
    if (!loadGraphicLibrary(getGraphicLibraryPath(ac, av)))
        return 84;
    _isRunning = true;
    setupMenuDefaults(ac, av);

    while (graphics().isOpen()) {
        if (_appState == MENU && graphics().consumeMenuConnectionEdited()) {
            _network.disconnect();
            graphics().resetMenuTeamNames();
            resetMenuLoadState();
        }
        if (_appState == MENU && graphics().handleMenuEvents()) {
            if (_network.isConnected() && _menuReady)
                _appState = GAME;
            else if (_network.isConnected())
                graphics().setMenuStatus("Loading map and teams...");
            else
                connectFromMenu();
        }
        if (_appState == GAME && graphics().handleGameEvents(_world)) {
            _network.disconnect();
            graphics().setMenuStatus("Disconnected");
            _appState = MENU;
        }
        if (_appState == GAME)
            sendPendingTimeRequest();
        if (_appState == GAME)
            sendPendingTileRequest();

        if (_network.isConnected()) {
            std::string command = _network.receiveData();
            if (!command.empty()) {
                parseCommand(command);
            }
        }
        _world.updateAnimations();
 
        graphics().clear();

        if (_appState == MENU)
            graphics().drawMenu();
        else if (_appState == GAME)
            graphics().drawWorld(_world);

        graphics().display();
    }

    return 0;
}

std::string Core::getGraphicLibraryPath(int ac, char** av) const
{
    const std::string defaultPath = "./libzappy_graphic_2d.so";

    for (int i = 1; i < ac; i++) {
        const std::string arg = av[i];

        if (isGraphicPathOption(arg) && i + 1 < ac)
            return av[i + 1];
        if (arg.rfind("--graphic=", 0) == 0)
            return arg.substr(std::string("--graphic=").size());
        if (arg.rfind("--graphic-lib=", 0) == 0)
            return arg.substr(std::string("--graphic-lib=").size());
    }
    return defaultPath;
}

bool Core::loadGraphicLibrary(const std::string& path)
{
    if (_graphicLibrary.load(path))
        return true;
    std::cerr << "[Core] Cannot load graphic library: " << path << std::endl;
    std::cerr << "[Core] " << _graphicLibrary.getLastError() << std::endl;
    std::cerr << "[Core] Build it with the GameEngine CMake target "
        << "zappy_graphic_2d, or pass another library with -g <path>."
        << std::endl;
    return false;
}

IGraphic& Core::graphics()
{
    return _graphicLibrary.get();
}

void Core::setupMenuDefaults(int ac, char** av)
{
    // const std::string port = (ac > 1) ? av[1] : "4242";
    // const std::string ip = (ac > 2) ? av[2] : "127.0.0.1";
    std::string port = "4242";
    std::string ip = "127.0.0.1";
    int positionalArgument = 0;

    for (int i = 1; i < ac; i++) {
        std::string arg = av[i];

        if (isGraphicPathOption(arg) && i + 1 < ac) {
            i++;
        } else if (arg.rfind("--graphic=", 0) == 0 ||
            arg.rfind("--graphic-lib=", 0) == 0) {
            continue;
        } else if ((arg == "-p" || arg == "--port") && i + 1 < ac) {
            port = av[i + 1];
            i++;
        } else if ((arg == "-h" || arg == "--host" || arg == "--machine")
            && i + 1 < ac) {
            ip = av[i + 1];
            i++;
        } else if (!arg.empty() && arg.front() == '-') {
            continue;
        } else if (positionalArgument == 0) {
            port = arg;
            positionalArgument++;
        } else if (positionalArgument == 1) {
            ip = arg;
            positionalArgument++;
        }
    }

    graphics().setMenuConnectionInfo(ip, port);
}

bool Core::connectFromMenu()
{
    const std::string ip = graphics().getMenuIp();
    const std::string portInput = graphics().getMenuPort();
    int parsedPort = 0;

    if (ip.empty() || portInput.empty()) {
        graphics().setMenuStatus("IP and port are required");
        return false;
    }
    try {
        std::size_t parsedSize = 0;

        parsedPort = std::stoi(portInput, &parsedSize);
        if (parsedSize != portInput.size() || parsedPort <= 0 ||
            parsedPort > std::numeric_limits<unsigned short>::max()) {
            graphics().setMenuStatus("Invalid port");
            return false;
        }
    } catch (const std::exception&) {
        graphics().setMenuStatus("Invalid port");
        return false;
    }
    graphics().setMenuStatus("Connecting...");
    _world = GameWorld();
    graphics().resetMenuTeamNames();
    resetMenuLoadState();
    if (!_network.connectToServer(ip, static_cast<unsigned short>(parsedPort))) {
        graphics().setMenuStatus("Connection failed");
        return false;
    }
    if (!_network.authenticateAsGraphic()) {
        _network.disconnect();
        graphics().setMenuStatus("Authentication failed");
        return false;
    }
    graphics().setMenuStatus("Loading map and teams...");
    std::cout << "[Core] Moteur démarré. Écoute du serveur..." << std::endl;
    return true;
}

void Core::resetMenuLoadState()
{
    _menuReady = false;
    _hasMenuTeams = false;
    _expectedInitialTiles = 0;
    _loadedInitialTiles.clear();
}

void Core::updateMenuReadyStatus()
{
    if (_menuReady || _expectedInitialTiles <= 0 || !_hasMenuTeams)
        return;
    if (static_cast<int>(_loadedInitialTiles.size()) < _expectedInitialTiles)
        return;
    _menuReady = true;
    graphics().setMenuStatus("Server ready - Press to enter");
}

void Core::trackInitialTileLoaded(int x, int y)
{
    if (_expectedInitialTiles <= 0 || _menuReady)
        return;
    if (x < 0 || x >= _world.getWidth() || y < 0 || y >= _world.getHeight())
        return;
    _loadedInitialTiles.insert({x, y});
}

void Core::sendPendingTileRequest()
{
    std::optional<std::pair<int, int>> request = graphics().consumeTileRequest();

    if (!request.has_value())
        return;

    const std::string command = "bct " + std::to_string(request->first) +
        " " + std::to_string(request->second) + "\n";

    _network.sendCommand(command);
}

void Core::sendPendingTimeRequest()
{
    std::optional<int> request = graphics().consumeTimeRequest();

    if (!request.has_value())
        return;

    const std::string command = "sst " + std::to_string(*request) + "\n";

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
        _expectedInitialTiles = std::max(0, width * height);
        _loadedInitialTiles.clear();
        updateMenuReadyStatus();
    };

    _commands["tna"] = [this](std::stringstream& ss) {
        std::string teamName;

        ss >> teamName;
        graphics().addMenuTeamName(teamName);
        _world.addTeamName(teamName);
        if (!teamName.empty())
            _hasMenuTeams = true;
        updateMenuReadyStatus();
    };

    _commands["pnw"] = [this](std::stringstream& ss) {
        Player p;
        parseValue(ss, p.id);
        parseValue(ss, p.x);
        parseValue(ss, p.y);
        parseValue(ss, p.orientation);
        parseValue(ss, p.level);
        ss >> p.teamName;
        graphics().addMenuTeamName(p.teamName);
        _world.addPlayer(p);
        if (!p.teamName.empty())
            _hasMenuTeams = true;
        updateMenuReadyStatus();
    };

    _commands["ppo"] = [this](std::stringstream& ss) {
        int id = 0, x = 0, y = 0, orientation = 0;
        parseValue(ss, id);
        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, orientation);
        _world.movePlayer(id, x, y, orientation);
    };

    _commands["sgt"] = [this](std::stringstream& ss) {
        int freq = 0;

        parseValue(ss, freq);
        graphics().setServerFreq(freq);
    };

    _commands["plv"] = [this](std::stringstream& ss) {
        int id = 0, level = 0;

        parseValue(ss, id);
        parseValue(ss, level);
        _world.setPlayerLevel(id, level);
    };

    _commands["pin"] = [this](std::stringstream& ss) {
        int id = 0, x = 0, y = 0;
        int q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0;

        parseValue(ss, id);
        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, q0);
        parseValue(ss, q1);
        parseValue(ss, q2);
        parseValue(ss, q3);
        parseValue(ss, q4);
        parseValue(ss, q5);
        parseValue(ss, q6);
        _world.setPlayerInventory(id, x, y,
            _world.resToResources(q0, q1, q2, q3, q4, q5, q6));
    };

    _commands["pic"] = [this](std::stringstream& ss) {
        int x = 0, y = 0, level = 0;
        std::vector<int> playerIds;
        const int serverFreq = std::max(1, graphics().getServerFreq());
        const int durationMs = 300000 / serverFreq;

        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, level);
        playerIds = parseTrailingPlayerIds(ss);
        (void)level;
        _world.startIncantation(x, y, playerIds, durationMs);
    };

    _commands["pie"] = [this](std::stringstream& ss) {
        int x = 0, y = 0, result = 0;

        parseValue(ss, x);
        parseValue(ss, y);
        parseValue(ss, result);
        (void)result;
        _world.stopIncantation(x, y);
    };

    _commands["pbc"] = [this](std::stringstream& ss) {
        int id = 0;
        const int durationMs = 850;

        parseValue(ss, id);
        _world.triggerBroadcast(id, durationMs);
    };

    _commands["pdi"] = [this](std::stringstream& ss) {
        int id = 0;
        const int durationMs = 700;

        parseValue(ss, id);
        _world.triggerPlayerDeath(id, durationMs);
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
        trackInitialTileLoaded(x, y);
        updateMenuReadyStatus();
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

    _commands["pdi"] = [this](std::stringstream& ss) {
        int id = 0;
        parseValue(ss, id);
        std::cout << "[Graphic] pdi handler: id=" << id << std::endl;
        _world.removePlayer(id);
    };

    _commands["pgt"] = [this](std::stringstream& ss) {
        int id = 0, resourceIndex = 0;
        parseValue(ss, id);
        parseValue(ss, resourceIndex);
        const auto& players = _world.getPlayers();
        auto it = players.find(id);
        if (it != players.end()) {
            _world.adjustTileResource(it->second.x, it->second.y, resourceIndex, -1);
        }
    };

    _commands["pdr"] = [this](std::stringstream& ss) {
        int id = 0, resourceIndex = 0;
        parseValue(ss, id);
        parseValue(ss, resourceIndex);
        const auto& players = _world.getPlayers();
        auto it = players.find(id);
        if (it != players.end()) {
            _world.adjustTileResource(it->second.x, it->second.y, resourceIndex, +1);
        }
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
