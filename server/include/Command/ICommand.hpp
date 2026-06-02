/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ICommand.hpp
*/

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

class MapCommand;
class PlayerCommand;
class GameCommand;

class ICommand {
    public:
        virtual ~ICommand() = default;

        virtual void visit(const MapCommand *element) const = 0;
        virtual void visit(const PlayerCommand *element) const = 0;
        virtual void visit(const GameCommand *element) const = 0;
};

#endif