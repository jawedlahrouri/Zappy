/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_forward.cpp
*/

#include "Parsing/AiCommandParser.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void AiCommandParser::parse_forward(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
    (void)client;
    (void)command;
    (void)server;
    std::cout << "Executing AI parse_forward" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_forward failed: ") + e.what());
    }
}
