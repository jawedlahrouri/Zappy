/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.cpp
*/

#include "ServerException.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Network/INetworkProvider.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "Client/AI.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include "shared/ILogger.hpp"

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
