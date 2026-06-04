/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ai_execute_cmds.cpp
*/

#include "Parsing/Parsing.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "Server/Server.hpp"
#include "Command/ICommand.hpp"
#include "Command/MapCommand.hpp"
#include "Command/PlayerCommand.hpp"
#include "Command/GameCommand.hpp"
#include <iostream>

std::map<std::string, void (AiCommandParser::*)(std::shared_ptr<AI>, const std::string &command)> ai_cmds = {
    {"Forward", &AiCommandParser::parse_forward},
    {"Right", &AiCommandParser::parse_right},
    {"Left", &AiCommandParser::parse_left},
    {"Look", &AiCommandParser::parse_look},
    {"Inventory", &AiCommandParser::parse_inventory},
    {"Broadcast", &AiCommandParser::parse_broadcast},
    {"Connect_nbr", &AiCommandParser::parse_connect_nbr},
    {"Fork", &AiCommandParser::parse_fork},
    {"Eject", &AiCommandParser::parse_eject},
    {"Take", &AiCommandParser::parse_take},
    {"Set", &AiCommandParser::parse_set},
    {"Incantation", &AiCommandParser::parse_incantation},
};