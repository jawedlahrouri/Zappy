/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.cpp
*/

#include "Command/GUI/PpoCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PpoCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("PpoCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_ppo(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        PpoCommand cmd;
        cmd.setClient(client);

        size_t space_pos = command.find(' ');
        if (space_pos != std::string::npos) {
            std::string id_str = command.substr(space_pos + 1);
            if (!id_str.empty() && id_str[0] == '#')
                id_str = id_str.substr(1);
            cmd.setPlayerId(std::stoi(id_str));
        }

        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_ppo failed: ") + e.what());
    }
}

void GuiVisitor::visit(PpoCommand *element) {
    try {
    auto gui_client = element->getClient();
    int target_id = element->getPlayerId();

    const auto &clients = _server.get_clients();
    auto it = clients.find(target_id);
    if (it != clients.end() && it->second->type == ClientType::AI) {
        auto ai = std::dynamic_pointer_cast<AI>(it->second);
        std::string ppo_msg = "ppo " + std::to_string(ai->id) + " " +
                              std::to_string(ai->x) + " " +
                              std::to_string(ai->y) + " " +
                              std::to_string(ai->orientation);
        Respond::send_response(_server, gui_client->socket.get_socket(), ppo_msg);
    } else {
        Respond::send_response(_server, gui_client->socket.get_socket(), "sbp"); // Server Bad Parameter
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GuiVisitor::visit: ") + e.what());
    }
}
