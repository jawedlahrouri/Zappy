/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** parse_gui_pgt.cpp
*/

#include "Parsing/GuiCommandParser.hpp"
#include "Exception_Server.hpp"
#include "Server/Server.hpp"
#include <iostream>

void GuiCommandParser::parse_gui_pgt(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
    (void)client;
    (void)command;
    (void)server;
    std::cout << "Executing GUI parse_gui_pgt" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_pgt failed: ") + e.what());
    }
}
