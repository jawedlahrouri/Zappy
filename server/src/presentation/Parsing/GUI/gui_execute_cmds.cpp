/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** gui_execute_cmds.cpp
*/

#include "Parsing.hpp"
#include "GuiCommandParser.hpp"
#include "gui_execute_cmds.hpp"
#include "application/Server/Server.hpp"

#include <iostream>

std::map<std::string, void (GuiCommandParser::*)(std::shared_ptr<GUI>, const std::string &command, INetworkProvider &network)> gui_cmds = {
    {"msz", &GuiCommandParser::parse_gui_msz},
    {"bct", &GuiCommandParser::parse_gui_bct},
    {"mct", &GuiCommandParser::parse_gui_mct},
    {"tna", &GuiCommandParser::parse_gui_tna},
    {"ppo", &GuiCommandParser::parse_gui_ppo},
    {"plv", &GuiCommandParser::parse_gui_plv},
    {"pin", &GuiCommandParser::parse_gui_pin},
    {"sgt", &GuiCommandParser::parse_gui_sgt},
    {"sst", &GuiCommandParser::parse_gui_sst},
};