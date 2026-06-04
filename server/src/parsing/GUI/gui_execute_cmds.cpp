/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** gui_execute_cmds.cpp
*/

#include "Parsing/Parsing.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Parsing/gui_execute_cmds.hpp"
#include "Server/Server.hpp"
#include "Command/ICommand.hpp"
#include "Command/MapCommand.hpp"
#include "Command/PlayerCommand.hpp"
#include "Command/GameCommand.hpp"
#include <iostream>

std::map<std::string, void (GuiCommandParser::*)(std::shared_ptr<GUI>, const std::string &command, Server &server)> gui_cmds = {
    {"msz", &GuiCommandParser::parse_gui_msz},
    {"bct", &GuiCommandParser::parse_gui_bct},
    {"mct", &GuiCommandParser::parse_gui_mct},
    {"tna", &GuiCommandParser::parse_gui_tna},
    {"ppo", &GuiCommandParser::parse_gui_ppo},
    {"pic", &GuiCommandParser::parse_gui_pic},
    {"pie", &GuiCommandParser::parse_gui_pie},
    {"plv", &GuiCommandParser::parse_gui_plv},
    {"pin", &GuiCommandParser::parse_gui_pin},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
    {"pgt", &GuiCommandParser::parse_gui_pgt},
    {"pbc", &GuiCommandParser::parse_gui_pbc},
    {"pfc", &GuiCommandParser::parse_gui_pfc},
    {"pfk", &GuiCommandParser::parse_gui_pfk},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
    {"pdr", &GuiCommandParser::parse_gui_pdr},
};