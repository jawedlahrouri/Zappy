/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LeftCommand.cpp
*/

#include "application/Command/AI/LeftCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "application/Event/GUI/PpoEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include <iostream>

void LeftCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("LeftCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_left([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        LeftCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_left failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] LeftCommand *element) {
    try {
        auto client = element->getClient();
        int orientation = client->getOrientation();

        if (orientation == 1) // NORTH
            client->setOrientation(4);
        else if (orientation == 2) // EAST
            client->setOrientation(1);
        else if (orientation == 3) // SOUTH
            client->setOrientation(2);
        else if (orientation == 4) // WEST
            client->setOrientation(3);
            
        _network.sendResponse(client->getId(), "ok\n");
        
        PpoEvent ppoEvent(client->getId(), client->getX(), client->getY(), client->getOrientation());
        EventVisitor eventVisitor(_network);
        ppoEvent.Accept(&eventVisitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
