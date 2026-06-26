/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ai_execute_cmds.cpp
*/


#include "AiCommandParser.hpp"
#include "ai_execute_cmds.hpp"

/**
 * @brief AI command dispatch table: name -> {parser method, tick duration}.
 * @details The durations are the protocol costs (in time units, divided by the
 *          server frequency): Forward/Right/Left/Look/Broadcast/Eject/Take/Set
 *          = 7, Inventory = 1, Connect_nbr = 0, Fork = 42, Incantation = 300.
 */
std::map<std::string, AiCommand> ai_cmds = {
    {"Forward", {&AiCommandParser::parse_forward, 7}},
    {"Right", {&AiCommandParser::parse_right, 7}},
    {"Left", {&AiCommandParser::parse_left, 7}},
    {"Look", {&AiCommandParser::parse_look, 7}},
    {"Inventory", {&AiCommandParser::parse_inventory, 1}},
    {"Broadcast", {&AiCommandParser::parse_broadcast, 7}},
    {"Connect_nbr", {&AiCommandParser::parse_connect_nbr, 0}},
    {"Fork", {&AiCommandParser::parse_fork, 42}},
    {"Eject", {&AiCommandParser::parse_eject, 7}},
    {"Take", {&AiCommandParser::parse_take, 7}},
    {"Set", {&AiCommandParser::parse_set, 7}},
    {"Incantation", {&AiCommandParser::parse_incantation, 300}},
};