/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameCommand.hpp
*/

#ifndef GAMECOMMAND_HPP
#define GAMECOMMAND_HPP

#include "./ICommand.hpp"

class GameCommand {
    public:
        virtual ~GameCommand() {}
        virtual void Accept(ICommand *element) const = 0;
};

#endif