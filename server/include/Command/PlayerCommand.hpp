/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlayerCommand.hpp
*/

#ifndef PLAYERCOMMAND_HPP
#define PLAYERCOMMAND_HPP

#include "./ICommand.hpp"

class PlayerCommand {
    public:
        virtual ~PlayerCommand() {}
        virtual void Accept(ICommand *element) const = 0;
};

#endif