/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** gui_parsing.cpp
*/

#include "Parsing/Parsing.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Parsing/gui_execute_cmds.hpp"
#include "Server/Server.hpp"
#include <iostream>

void Parsing::parse_gui_command(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    std::vector<std::string> tokens = split_command(command);
    if (tokens.empty())
        return;

    std::string req = tokens[0];
    auto it = gui_cmds.find(req);
    if (it != gui_cmds.end()) {
        GuiCommandParser parser;
        auto func = it->second;
        (parser.*func)(client, command, server);
    } else {
        client->write_buffer += "suc\n";
    }
}
