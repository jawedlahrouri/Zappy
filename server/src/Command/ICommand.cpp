/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ICommand.cpp
*/

#include "Command/ICommand.hpp"
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, void (ICommand::*)(std::string)> commands = {

};