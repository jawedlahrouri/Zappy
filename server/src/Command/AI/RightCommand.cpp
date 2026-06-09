/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.cpp
*/

#include "Command/AI/RightCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void RightCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("RightCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_right(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)command;
        (void)server;
        RightCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_right failed: ") + e.what());
    }
}

void AiVisitor::visit(RightCommand *element) {
    try {
        auto client = element->getClient();
        client->orientation = (client->orientation + 1) % 4;
        if (client->orientation == 0)
            client->orientation = 4;
        Respond::send_response(_server, client->socket.get_socket(), "ok");
        std::string ppo_msg = "ppo " + std::to_string(client->id) + " " +
                                std::to_string(client->x) + " " +
                                std::to_string(client->y) + " " +
                                std::to_string(client->orientation);
        Respond::send_to_guis(_server, ppo_msg);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
