/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_inventory.cpp
*/

#include "Parsing/AiCommandParser.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void AiCommandParser::parse_inventory(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
    (void)client;
    (void)command;
    (void)server;
    std::cout << "Executing AI parse_inventory" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_inventory failed: ") + e.what());
    }
}
