/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.cpp
*/

#include "Exception_Server.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Server/Server.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "Parsing/Respond.hpp"
#include <iostream>
#include <random>
#include <cmath>

GameEngine::GameEngine(int width, int height)
    : _width(width), _height(height)
{
    try {
        _map.resize(_height, std::vector<Tile>(_width));
        generate_initial_resources();
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::GameEngine: ") + e.what());
    }
}

void GameEngine::update(Server &server)
{
    try {
        std::vector<int> clients_to_remove;
        const auto &clients = server.get_clients();

    for (auto &[fd, client] : clients) {
        client->update(client, server, *this, clients_to_remove);
    }

        for (int fd : clients_to_remove) {
            server.remove_client(fd);
        }
        _resource_spawn_ticks++;

        if (_resource_spawn_ticks >= 20) {
            spawn_resources_cycle();
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
                    Respond::send_to_guis(server, bct_msg);
                }
            }
            _resource_spawn_ticks = 0;
            std::cout << "[MAP] Resources respawn cycle triggered and pushed to GUIs." << std::endl;
        }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::update: ") + e.what());
    }
}

void GameEngine::update_ai(std::shared_ptr<AI> ai, Server &server, std::vector<int> &clients_to_remove)
{
    try {
    if (!handle_ai_survival(ai->id, ai, server, clients_to_remove))
        return;
    handle_ai_active_command(ai, server);
    handle_ai_next_command(ai, server);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::update_ai: ") + e.what());
    }
}

void GameEngine::spawn_resources_cycle()
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

bool GameEngine::handle_ai_survival(int fd, std::shared_ptr<AI> ai, Server &server, std::vector<int> &clients_to_remove)
{
    try {
    ai->food_ticks--;
    if (ai->food_ticks <= 0) {
        if (ai->inventory["food"] > 0) {
            ai->inventory["food"]--;
            ai->food_ticks = 126;
            return true;
        }
        Respond::send_response(server, fd, "death");
        clients_to_remove.push_back(fd);
        return false;
    }
    return true;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::handle_ai_survival: ") + e.what());
    }
}

void GameEngine::handle_ai_active_command(std::shared_ptr<AI> ai, Server &server)
{
    try {
    if (ai->action_ticks_left > 0) {
        ai->action_ticks_left--;
        if (ai->action_ticks_left == 0) {
            execute_ai_action(ai, server);
            ai->action_ticks_left = -1;
        }
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::handle_ai_active_command: ") + e.what());
    }
}

void GameEngine::handle_ai_next_command(std::shared_ptr<AI> ai, Server &server)
{
    try {
    if (ai->action_ticks_left == -1 && !ai->commandQueue.empty()) {
        std::string next_cmd = ai->commandQueue.front();
        ai->action_ticks_left = get_command_duration(next_cmd);
        if (ai->action_ticks_left == 0) {
            execute_ai_action(ai, server);
            ai->action_ticks_left = -1;
        }
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::handle_ai_next_command: ") + e.what());
    }
}

void GameEngine::execute_ai_action(std::shared_ptr<AI> ai, Server &server)
{
    try {
    AiCommandParser parser;

    if (ai->commandQueue.empty())
        return;
    std::string command = ai->commandQueue.front();
    ai->commandQueue.pop();

    std::string action = command.substr(0, command.find(' '));
    auto it = ai_cmds.find(action);
    if (it != ai_cmds.end()) {
        auto func = it->second.func;
        (parser.*func)(ai, command, server);
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::execute_ai_action: ") + e.what());
    }
}

int GameEngine::get_command_duration(const std::string &command)
{
    try {
    std::string action = command.substr(0, command.find(' '));
    auto it = ai_cmds.find(action);
    if (it != ai_cmds.end()) {
        return it->second.duration;
    }
    return 0;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::get_command_duration: ") + e.what());
    }
}

const Tile& GameEngine::get_tile(int x, int y) const
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    return _map[toric_y][toric_x];
}

void GameEngine::add_resource_to_tile(int x, int y, ResourceType type, int quantity)
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    _map[toric_y][toric_x].resources[type] += quantity;
}

void GameEngine::remove_resource_from_tile(int x, int y, ResourceType type, int quantity)
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;

    _map[toric_y][toric_x].resources[type] -= quantity;
    if (_map[toric_y][toric_x].resources[type] < 0)
        _map[toric_y][toric_x].resources[type] = 0;
}

void GameEngine::generate_initial_resources()
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
        std::cout << "[MAP] Initial resources generated successfully." << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GameEngine::generate_initial_resources: ") + e.what());
    }
}
