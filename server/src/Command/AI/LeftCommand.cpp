/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LeftCommand.cpp
*/

#include "Command/AI/LeftCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void LeftCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("LeftCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_left([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] Server &server)
{
    try {
        LeftCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_left failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] LeftCommand *element) {
    try {
        auto client = element->getClient();
        client->orientation = (client->orientation - 1 + 4) % 4;
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
