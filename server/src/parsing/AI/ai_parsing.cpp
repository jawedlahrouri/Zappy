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


std::vector<std::string> Parsing::split_command(const std::string &command)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(command);

    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void Parsing::parse_ai_command(std::shared_ptr<AI> client, const std::string &command)
{
    std::vector<std::string> tokens = split_command(command);
    if (tokens.empty())
        return;

    std::string action = tokens[0];
    if (ai_cmds.find(action) == ai_cmds.end()) {
        client->write_buffer += "ko\n";
        return;
    }

    // queue 10 commands
    if (client->commandQueue.size() < 10) {
        client->commandQueue.push(command);
        std::cout << "AI [#" << client->id << "] queued command: " << command << std::endl;
    } else {
        std::cout << "AI [#" << client->id << "] command queue full, command dropped" << std::endl;
    }
}
