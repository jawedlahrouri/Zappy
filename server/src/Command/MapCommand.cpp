/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CommandMap.cpp
*/

#include "Command/ICommand.hpp"

class CommandMap : public ICommand {
public:
    virtual ~CommandMap() = default;

    virtual void visit(const MapCommand *element) const {
        (void)element;
    };
};
