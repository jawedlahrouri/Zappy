/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_broadcast.cpp
*/

#include "Parsing/AiCommandParser.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void AiCommandParser::parse_broadcast(std::shared_ptr<AI> client, const std::string &command)
{
    try {
    (void)client;
    (void)command;
    std::cout << "Executing AI parse_broadcast" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_broadcast failed: ") + e.what());
    }
}
