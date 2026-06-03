/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Command.cpp
*/

#include "Command/ICommand.hpp"
#include "Command/MapCommand.hpp"
#include "Command/PlayerCommand.hpp"
#include "Command/GameCommand.hpp"
#include <iostream>

void Command::visit(const MapCommand *element) const {
    (void)element;
    std::cout << "Executing Map Command" << std::endl;
}

void Command::visit(const PlayerCommand *element) const {
    (void)element;
    std::cout << "Executing Player Command" << std::endl;
}

void Command::visit(const GameCommand *element) const {
    (void)element;
    std::cout << "Executing Game Command" << std::endl;
}
