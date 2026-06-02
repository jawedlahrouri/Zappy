/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MapCommand.hpp
*/

#ifndef MAPCOMMAND_HPP
#define MAPCOMMAND_HPP

#include "ICommand.hpp"

class MapCommand {
public:
    virtual ~MapCommand() {}
    virtual void Accept(ICommand *element) const = 0;
};

#endif