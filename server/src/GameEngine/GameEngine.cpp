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

GameEngine::GameEngine(int width, int height)
    : _width(width), _height(height)
{
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
