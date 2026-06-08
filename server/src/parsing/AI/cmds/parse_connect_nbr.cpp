/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_connect_nbr.cpp
*/

#include "Parsing/AiCommandParser.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void AiCommandParser::parse_connect_nbr(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
    (void)client;
    (void)command;
    (void)server;
    std::cout << "Executing AI parse_connect_nbr" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_connect_nbr failed: ") + e.what());
    }
}
