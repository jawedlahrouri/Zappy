/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_take.cpp
*/

#include "Parsing/AiCommandParser.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void AiCommandParser::parse_take(std::shared_ptr<AI> client, const std::string &command)
{
    try {
    (void)client;
    (void)command;
    std::cout << "Executing AI parse_take" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_take failed: ") + e.what());
    }
}
