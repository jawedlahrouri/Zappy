/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinCommand.cpp
*/
#include "application/Command/GUI/PinCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/AI.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"
#include <iostream>
#include <sstream>

void PinCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("PinCommand::Accept: ") + e.what());
    }
}

void PinCommand::setPlayerId(int id)
{
    _playerId = id;
}

int PinCommand::getPlayerId() const
{
    return _playerId;
}

void GuiCommandParser::parse_gui_pin(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::istringstream iss(command);
        std::string cmd;
        int targetId = 0;

        if (!(iss >> cmd >> targetId)) {
            network.sendResponse(client->getId(), "sbp");
            return;
        }
        PinCommand pinCmd;
        pinCmd.setPlayerId(targetId);
        pinCmd.setClient(client);
        GuiVisitor visitor(network);
        pinCmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_pin failed: ") + e.what());
    }
}

/**
 * @brief Replies with a player's inventory, or "sbp" if the id is unknown.
 * @par Protocol (GUI protocol)
 * Request "pin #n\n" -> Response "pin #n X Y q0 q1 q2 q3 q4 q5 q6\n".
 */
void GuiVisitor::visit(PinCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server)
            return;

        int targetId = element->getPlayerId();
        const auto& clients = server->getClients();
        auto it = clients.find(targetId);

        if (it == clients.end() || it->second->getType() != ClientType::AI) {
            _network.sendResponse(element->getClient()->getId(), "sbp");
            return;
        }

        auto ai = std::dynamic_pointer_cast<AI>(it->second);
        if (!ai)
            return;

        auto& inv = ai->getInventory();

        std::string response = "pin #" + std::to_string(ai->getId()) + " " +
                               std::to_string(ai->getX()) + " " +
                               std::to_string(ai->getY()) + " " +
                               std::to_string(inv["food"]) + " " +
                               std::to_string(inv["linemate"]) + " " +
                               std::to_string(inv["deraumere"]) + " " +
                               std::to_string(inv["sibur"]) + " " +
                               std::to_string(inv["mendiane"]) + " " +
                               std::to_string(inv["phiras"]) + " " +
                               std::to_string(inv["thystame"]);
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(PinCommand): ") + e.what());
    }
}