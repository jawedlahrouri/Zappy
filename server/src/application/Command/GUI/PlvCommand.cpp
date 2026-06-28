/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlvCommand.cpp
*/
#include "application/Command/GUI/PlvCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/AI.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

#include <iostream>
#include <sstream>

void PlvCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("PlvCommand::Accept: ") + e.what());
    }
}

void PlvCommand::setPlayerId(int id)
{
    _playerId = id;
}

int PlvCommand::getPlayerId() const
{
    return _playerId;
}

void GuiCommandParser::parse_gui_plv(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::istringstream iss(command);
        std::string cmd;
        int targetId = 0;

        if (!(iss >> cmd >> targetId)) {
            network.sendResponse(client->getId(), "sbp");
            return;
        }
        PlvCommand plvCmd;
        plvCmd.setPlayerId(targetId);
        plvCmd.setClient(client);
        GuiVisitor visitor(network);
        plvCmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_plv failed: ") + e.what());
    }
}

/**
 * @brief Replies with a player's level, or "sbp" if the id is unknown.
 * @par Protocol (GUI protocol)
 * Request "plv #n\n" -> Response "plv #n L\n".
 */
void GuiVisitor::visit(PlvCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server) return;

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

        std::string response = "plv #" + std::to_string(ai->getId()) + " " +
                               std::to_string(ai->getLevel());
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(PlvCommand): ") + e.what());
    }
}