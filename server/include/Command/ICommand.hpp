/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ICommand.hpp
*/

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

class Server;
class MapCommand;
class PlayerCommand;
class GameCommand;

class ICommand {
    public:
        virtual ~ICommand() = default;

        virtual void visit(MapCommand *element) const = 0;
        virtual void visit(PlayerCommand *element) const = 0;
        virtual void visit(GameCommand *element) const = 0;
};

class Command : public ICommand {
    public:
        Command(Server &server);
        ~Command() override = default;

        void visit(MapCommand *element) const override;
        void visit(PlayerCommand *element) const override;
        void visit(GameCommand *element) const override;
    private:
        Server &_server;
};

#endif