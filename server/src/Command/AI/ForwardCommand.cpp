/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForwardCommand.cpp
*/

#include "Command/AI/ForwardCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include "GameEngine/GameEngine.hpp"
#include <iostream>

void ForwardCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("ForwardCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_forward(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)command;
        ForwardCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_forward failed: ") + e.what());
    }
}

void AiVisitor::visit(ForwardCommand *element) {
    try {
        auto client = element->getClient();
        auto engine = _server.get_game_engine().get();
        int width = engine->get_width();
        int height = engine->get_height();

        if (client->orientation == 1) {
            client->y = (client->y - 1 + height) % height;
        } else if (client->orientation == 2) {
            client->x = (client->x + 1) % width;
        } else if (client->orientation == 3) {
            client->y = (client->y + 1) % height;
        } else if (client->orientation == 4) {
            client->x = (client->x - 1 + width) % width;
        }
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
