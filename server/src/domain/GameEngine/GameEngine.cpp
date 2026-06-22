/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.cpp
*/

#include "ServerException.hpp"
#include "GameEngine.hpp"
#include "INetworkProvider.hpp"
#include "AI.hpp"
#include "Client.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include "shared/ILogger.hpp"
#include "application/Event/GUI/BctEvent.hpp"
#include "application/Event/GUI/PfkEvent.hpp"
#include "application/Event/GUI/PicEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
Map& GameEngine::getMap()
{
    return _map;
}

const Map& GameEngine::getMap() const
{
    return _map;
}

int GameEngine::getWidth() const
{
    return _width;
}

int GameEngine::getHeight() const
{
    return _height;
}

GameEngine::GameEngine(int width, int height, ILogger& logger)
    : _logger(logger), _width(width), _height(height), _map(width, height)
{
    try {
        generateInitialResources();
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::GameEngine: ") + e.what());
    }
}

void GameEngine::update(INetworkProvider& network)
{
    try {
        _resourceSpawnTicks++;

        if (_resourceSpawnTicks >= 20) {
            spawnResourcesCycle();
            
            bool hasGui = false;
            for (const auto &[fd, client] : network.getClients()) {
                if (client && client->getType() == ClientType::GUI) {
                    hasGui = true;
                    break;
                }
            }

            if (hasGui) {
                for (int key : _map.getDirtyTiles()) {
                    int x = key % _width;
                    int y = key / _width;
                    const Tile &t = _map.getTile(x, y);
                    BctEvent bctEvent(x, y, t.resources[FOOD], t.resources[LINEMATE], t.resources[DERAUMERE],
                                    t.resources[SIBUR], t.resources[MENDIANE], t.resources[PHIRAS], t.resources[THYSTAME]);
                    EventVisitor visitor(network);
                    bctEvent.Accept(&visitor);
                }
            }

            _map.clearDirtyTiles();
            _resourceSpawnTicks = 0;
            _logger.info("[MAP] Resources respawn cycle triggered and pushed to GUIs.");
        }
    } catch (const std::exception& e) {
        throw ServerException(std::string("GameEngine::update: ") + e.what());
    }
}

void GameEngine::updateAi(std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove)
{
    try {
    if (!handleAiSurvival(ai->getId(), ai, network, clientsToRemove))
        return;
    handleAiActiveCommand(ai, network);
    handleAiNextCommand(ai, network);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::updateAi: ") + e.what());
    }
}

void GameEngine::spawnResourcesCycle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_x(0, _width - 1);
    std::uniform_int_distribution<> dis_y(0, _height - 1);

    for (int i = 0; i < RESOURCE_COUNT; i++) {
        ResourceType type = static_cast<ResourceType>(i);
        int target_quantity = std::max(1, static_cast<int>(std::floor(_width * _height * _densities[i])));
        int current_quantity = _map.countResource(type);
        int missing = target_quantity - current_quantity;

        for (int q = 0; q < missing; q++) {
            _map.addResourceToTile(dis_x(gen), dis_y(gen), type, 1);
        }
    }
}

bool GameEngine::handleAiSurvival(int fd, std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove)
{
    try {
    ai->setFoodTicks(ai->getFoodTicks() - 1);
    if (ai->getFoodTicks() <= 0) {
        if (ai->getInventory()["food"] > 0) {
            ai->getInventory()["food"]--;
            ai->setFoodTicks(126);
            return true;
        }
        network.sendResponse(fd, "death");
        clientsToRemove.push_back(fd);
        return false;
    }
    return true;
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::handleAiSurvival: ") + e.what());
    }
}

void GameEngine::handleAiActiveCommand(std::shared_ptr<AI> ai, INetworkProvider& network)
{
    try {
    if (ai->getActionTicksLeft() > 0) {
        ai->setActionTicksLeft(ai->getActionTicksLeft() - 1);
        if (ai->getActionTicksLeft() == 0) {
            executeAiAction(ai, network);
            ai->setActionTicksLeft(-1);
        }
    }
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::handleAiActiveCommand: ") + e.what());
    }
}

void GameEngine::handleAiNextCommand(std::shared_ptr<AI> ai, INetworkProvider& network)
{
    try {
    if (ai->getActionTicksLeft() == -1 && !ai->getCommandQueue().empty()) {
        std::string next_cmd = ai->getCommandQueue().front();

        if (next_cmd == "Incantation") {
            if (!checkIncantationStart(ai, network)) {
                ai->getCommandQueue().pop();
                return;
            }
        }
        if (next_cmd == "Fork") {
            PfkEvent pfkEvent(ai->getId());
            EventVisitor visitor(network);
            pfkEvent.Accept(&visitor);
            _logger.debug("Fork processed for AI #" + std::to_string(ai->getId()));
            ai->getCommandQueue().pop();
            return;
        }

        ai->setActionTicksLeft(network.getCommandDuration(next_cmd));
        if (ai->getActionTicksLeft() == 0) {
            executeAiAction(ai, network);
            ai->setActionTicksLeft(-1);
        }
    }
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::handleAiNextCommand: ") + e.what());
    }
}

void GameEngine::executeAiAction(std::shared_ptr<AI> ai, INetworkProvider& network)
{
    try {
    if (ai->getCommandQueue().empty())
        return;
    std::string command = ai->getCommandQueue().front();
    ai->getCommandQueue().pop();

    network.executeCommand(ai, command);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::executeAiAction: ") + e.what());
    }
}




const std::unordered_map<int, GameEngine::IncantationRequirement> GameEngine::_incantationReqs = {
    {1, {1, {0, 1, 0, 0, 0, 0, 0}}},
    {2, {2, {0, 1, 1, 1, 0, 0, 0}}},
    {3, {2, {0, 2, 0, 1, 0, 2, 0}}},
    {4, {4, {0, 1, 1, 2, 0, 1, 0}}},
    {5, {4, {0, 1, 2, 1, 3, 0, 0}}},
    {6, {6, {0, 1, 2, 3, 0, 1, 0}}},
    {7, {6, {0, 2, 2, 2, 2, 2, 1}}}
};

std::vector<std::shared_ptr<AI>> GameEngine::getPlayersOnTileForIncantation(int x, int y, int level, const INetworkProvider& network) const
{
    std::vector<std::shared_ptr<AI>> players;
    const Tile& t = _map.getTile(x, y);
    for (int pid : t.playerIds) {
        auto it = network.getClients().find(pid);
        if (it != network.getClients().end()) {
            auto ai = std::dynamic_pointer_cast<AI>(it->second);
            if (ai && ai->getLevel() == level) {
                players.push_back(ai);
            }
        }
    }
    return players;
}

bool GameEngine::checkIncantationRequirements(int x, int y, int level) const
{
    auto reqIt = _incantationReqs.find(level);
    if (reqIt == _incantationReqs.end()) return false;
    const auto& req = reqIt->second;

    const Tile& t = _map.getTile(x, y);
    for (int i = 1; i < RESOURCE_COUNT; ++i) {
        if (t.resources[i] < req.resources[i]) {
            return false;
        }
    }
    return true;
}

void GameEngine::consumeIncantationResources(int x, int y, int level)
{
    auto reqIt = _incantationReqs.find(level);
    if (reqIt == _incantationReqs.end()) return;
    const auto& req = reqIt->second;

    for (int i = 1; i < RESOURCE_COUNT; ++i) {
        _map.removeResourceFromTile(x, y, static_cast<ResourceType>(i), req.resources[i]);
    }
}

void GameEngine::checkVictory(const std::string& teamName, INetworkProvider& network) const
{
    int count = 0;
    for (const auto& pair : network.getClients()) {
        auto ai = std::dynamic_pointer_cast<AI>(pair.second);
        if (ai && ai->getTeamName() == teamName && ai->getLevel() == 8) {
            count++;
        }
    }
    
    if (count >= 6) {
        network.notifyVictory(teamName);
    }
}

bool GameEngine::checkIncantationStart(std::shared_ptr<AI> ai, INetworkProvider& network)
{
    int level = ai->getLevel();
    int x = ai->getX();
    int y = ai->getY();

    auto players = getPlayersOnTileForIncantation(x, y, level, network);
    auto reqIt = _incantationReqs.find(level);
    
    if (reqIt == _incantationReqs.end() || players.size() < static_cast<size_t>(reqIt->second.players) || !checkIncantationRequirements(x, y, level)) {
        network.sendResponse(ai->getId(), "ko\n");
        return false;
    }

    std::vector<int> playerIds;
    for (const auto& p : players) {
        p->setActionTicksLeft(300);
        network.sendResponse(p->getId(), "Elevation underway\n");
        playerIds.push_back(p->getId());
    }
    
    PicEvent picEvent(x, y, level, playerIds);
    EventVisitor visitor(network);
    picEvent.Accept(&visitor);
    return true;
}



void GameEngine::generateInitialResources()
{
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, _width - 1);
        std::uniform_int_distribution<> dis_y(0, _height - 1);

        for (int i = 0; i < RESOURCE_COUNT; i++) {
            double density = _densities[i];
            int total_quantity = std::floor(_width * _height * density);
            
            if (total_quantity == 0 && _width > 0 && _height > 0) {
                total_quantity = 1;
            }
            for (int q = 0; q < total_quantity; q++) {
                int rand_x = dis_x(gen);
                int rand_y = dis_y(gen);
                _map.addResourceToTile(rand_x, rand_y, static_cast<ResourceType>(i), 1);
            }
        }
        _logger.info("[MAP] Initial resources generated successfully.");
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::generateInitialResources: ") + e.what());
    }
}

void GameEngine::generateInitialEggs(const std::vector<std::string>& teamNames, int initialSlots)
{
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, _width - 1);
        std::uniform_int_distribution<> dis_y(0, _height - 1);

        for (const auto& teamName : teamNames) {
            for (int i = 0; i < initialSlots; ++i) {
                int eggId = generateEggId();
                auto egg = std::make_shared<Egg>();
                egg->id = eggId;
                egg->ownerId = -1;
                egg->teamName = teamName;
                egg->x = dis_x(gen);
                egg->y = dis_y(gen);
                _map.addEgg(egg);
            }
        }
        _logger.info("[MAP] Initial eggs generated successfully.");
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::generateInitialEggs: ") + e.what());
    }
}

int GameEngine::getRequiredPlayersForIncantation(int level) const
{
    auto reqIt = _incantationReqs.find(level);
    if (reqIt != _incantationReqs.end()) {
        return reqIt->second.players;
    }
    return 0;
}
