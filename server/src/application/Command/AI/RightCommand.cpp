/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.cpp
*/

#include "application/Command/AI/RightCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "application/Event/GUI/PpoEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include <iostream>

void RightCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("RightCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_right(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network)
{
    (void)command;
    try {
        RightCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_right failed: ") + e.what());
    }
}

void AiVisitor::visit(RightCommand *element) {
    try {
        auto client = element->getClient();
        int orientation = client->getOrientation();

        if (orientation == 1) // NORTH
            client->setOrientation(2);
        else if (orientation == 2) // EAST
            client->setOrientation(3);
        else if (orientation == 3) // SOUTH
            client->setOrientation(4);
        else if (orientation == 4) // WEST
            client->setOrientation(1);
            
        _network.sendResponse(client->getId(), "ok\n");
        
        PpoEvent ppoEvent(client->getId(), client->getX(), client->getY(), client->getOrientation());
        EventVisitor eventVisitor(_network);
        ppoEvent.Accept(&eventVisitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
