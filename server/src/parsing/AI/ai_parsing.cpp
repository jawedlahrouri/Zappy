/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ai_parsing.cpp
*/

#include "Parsing/Parsing.hpp"
#include "Server/Server.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Parsing/ai_execute_cmds.hpp"
#include "Exception_Server.hpp"

std::vector<std::string> Parsing::split_command(const std::string &command)
{
    try {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(command);

        while (tokenStream >> token) {
            tokens.push_back(token);
        }
        return tokens;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("split_command failed: ") + e.what());
    }
}

void Parsing::parse_ai_command(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        std::vector<std::string> tokens = split_command(command);
        if (tokens.empty())
            return;

        std::string action = tokens[0];
        if (ai_cmds.find(action) == ai_cmds.end()) {
            server.send_response(client->socket.get_socket(), "ko");
            return;
        }

        // queue 10 commands
        if (client->commandQueue.size() < 10) {
            client->commandQueue.push(command);
            std::cout << "AI [#" << client->id << "] queued command: " << command << std::endl;
        } else {
            std::cout << "AI [#" << client->id << "] command queue full, command dropped" << std::endl;
        }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_ai_command failed: ") + e.what());
    }
}
