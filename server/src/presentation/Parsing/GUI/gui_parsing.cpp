/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** gui_parsing.cpp
*/

#include "Parsing.hpp"
#include "GuiCommandParser.hpp"
#include "gui_execute_cmds.hpp"
#include "application/Server/Server.hpp"
#include <iostream>
#include "ServerException.hpp"
#include "GUI.hpp"


void Parsing::parse_gui_command(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::vector<std::string> tokens = split_command(command);
        if (tokens.empty())
            return;

        std::string req = tokens[0];
        auto it = gui_cmds.find(req);
        if (it != gui_cmds.end()) {
            GuiCommandParser parser;
            auto func = it->second;
            (parser.*func)(client, command, network);
        } else {
            network.sendResponse(client->getId(), "suc");
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_command failed: ") + e.what());
    }
}
