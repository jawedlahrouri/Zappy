/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.cpp
*/

#include "ServerException.hpp"
#include "GameEngine.hpp"
#include "INetworkProvider.hpp"
#include "AiCommandParser.hpp"
#include "ai_execute_cmds.hpp"
#include "AI.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include "shared/ILogger.hpp"

int Egg::getId() const
{
    return id;
}

int Egg::getOwnerId() const
{
    return ownerId;
}

void Tile::addEgg(std::shared_ptr<Egg> egg)
{
    eggs.push_back(egg);
}

void Tile::removeEgg(int eggId)
{
    eggs.erase(std::remove_if(eggs.begin(), eggs.end(), [eggId](const std::shared_ptr<Egg>& egg){ return egg->getId() == eggId; }), eggs.end());
}

const std::vector<std::shared_ptr<Egg>>& Tile::getEggs() const
{
    return eggs;
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
    : _logger(logger), _width(width), _height(height)
{
    try {
        _map.resize(_height, std::vector<Tile>(_width));
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
            for (int y = 0; y < _height; y++) {
                for (int x = 0; x < _width; x++) {
                    const Tile &t = _map[y][x];
                    std::string bct_msg = "bct " + std::to_string(x) + " " + std::to_string(y) + " " +
                                          std::to_string(t.resources[FOOD]) + " " +
                                          std::to_string(t.resources[LINEMATE]) + " " +
                                          std::to_string(t.resources[DERAUMERE]) + " " +
                                          std::to_string(t.resources[SIBUR]) + " " +
                                          std::to_string(t.resources[MENDIANE]) + " " +
                                          std::to_string(t.resources[PHIRAS]) + " " +
                                          std::to_string(t.resources[THYSTAME]);
                    network.sendToGuis(bct_msg);
                }
            }
            _resourceSpawnTicks = 0;
            _logger.info("[MAP] Resources respawn cycle triggered and pushed to GUIs.");
        }
    } catch (const std::exception &e) {
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
        int amount_to_spawn = std::max(1, static_cast<int>(std::floor(_width * _height * _densities[i] * 0.1)));
        for (int q = 0; q < amount_to_spawn; q++) {
            _map[dis_y(gen)][dis_x(gen)].resources[i]++;
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

        ai->setActionTicksLeft(getCommandDuration(next_cmd));
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
    AiCommandParser parser;

    if (ai->getCommandQueue().empty())
        return;
    std::string command = ai->getCommandQueue().front();
    ai->getCommandQueue().pop();

    std::string action = command.substr(0, command.find(' '));
    auto it = ai_cmds.find(action);
    if (it != ai_cmds.end()) {
        auto func = it->second.func;
        (parser.*func)(ai, command, network);
    }
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::executeAiAction: ") + e.what());
    }
}

int GameEngine::getCommandDuration(const std::string& command)
{
    try {
    std::string action = command.substr(0, command.find(' '));
    auto it = ai_cmds.find(action);
    if (it != ai_cmds.end()) {
        return it->second.duration;
    }
    return 0;
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::getCommandDuration: ") + e.what());
    }
}

const Tile& GameEngine::getTile(int x, int y) const
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    return _map[toric_y][toric_x];
}

void GameEngine::addResourceToTile(int x, int y, ResourceType type, int quantity)
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    _map[toric_y][toric_x].resources[type] += quantity;
}

void GameEngine::removeResourceFromTile(int x, int y, ResourceType type, int quantity)
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    _map[toric_y][toric_x].resources[type] -= quantity;
    if (_map[toric_y][toric_x].resources[type] < 0)
        _map[toric_y][toric_x].resources[type] = 0;
}

void GameEngine::removeEgg(int eggId)
{
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            auto& eggs = _map[y][x].eggs;
            for (auto it = eggs.begin(); it != eggs.end(); ++it) {
                if ((*it)->getId() == eggId) {
                    eggs.erase(it);
                    return;
                }
            }
        }
    }
}

void GameEngine::addEgg(std::shared_ptr<Egg> egg)
{
    int toric_x = (egg->x % _width + _width) % _width;
    int toric_y = (egg->y % _height + _height) % _height;

    _map[toric_y][toric_x].addEgg(egg);
}

void GameEngine::addAiToTile(int aiId, int x, int y)
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    _map[toric_y][toric_x].playerIds.push_back(aiId);
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
    const Tile& t = getTile(x, y);
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

    const Tile& t = getTile(x, y);
    for (int i = 1; i < RESOURCE_COUNT; ++i) {
        if (t.resources[i] != req.resources[i]) {
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
        removeResourceFromTile(x, y, static_cast<ResourceType>(i), req.resources[i]);
    }
}

#include "application/Event/GUI/SegEvent.hpp"

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
        network.sendEvent(SegEvent(teamName));
    }
}

bool GameEngine::checkIncantationStart(std::shared_ptr<AI> ai, INetworkProvider& network)
{
    int level = ai->getLevel();
    int x = ai->getX();
    int y = ai->getY();

    auto players = getPlayersOnTileForIncantation(x, y, level, network);
    auto reqIt = _incantationReqs.find(level);
    
    if (reqIt == _incantationReqs.end() || players.size() != static_cast<size_t>(reqIt->second.players) || !checkIncantationRequirements(x, y, level)) {
        network.sendResponse(ai->getId(), "ko\n");
        return false;
    }

    std::string pic_msg = "pic " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(level);
    for (const auto& p : players) {
        network.sendResponse(p->getId(), "Elevation underway\n");
        pic_msg += " " + std::to_string(p->getId());
    }
    pic_msg += "\n";
    
    network.sendToGuis(pic_msg);
    return true;
}

void GameEngine::moveAi(int aiId, int oldX, int oldY, int newX, int newY)
{
    int toric_old_x = (oldX % _width + _width) % _width;
    int toric_old_y = (oldY % _height + _height) % _height;
    int toric_new_x = (newX % _width + _width) % _width;
    int toric_new_y = (newY % _height + _height) % _height;

    auto& oldPlayers = _map[toric_old_y][toric_old_x].playerIds;
    for (auto it = oldPlayers.begin(); it != oldPlayers.end(); ++it) {
        if (*it == aiId) {
            oldPlayers.erase(it);
            break;
        }
    }
    
    _map[toric_new_y][toric_new_x].playerIds.push_back(aiId);
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
                _map[rand_y][rand_x].resources[i]++;
            }
        }
        _logger.info("[MAP] Initial resources generated successfully.");
    } catch (const std::exception &e) {
        throw ServerException(std::string("GameEngine::generateInitialResources: ") + e.what());
    }
}
