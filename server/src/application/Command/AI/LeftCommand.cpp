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
        int orientation = element->getClient()->getOrientation();

        if (orientation == 0) // NORTH
            element->getClient()->setOrientation(3);
        else if (orientation == 1) // EAST
            element->getClient()->setOrientation(0);
        else if (orientation == 2) // SOUTH
            element->getClient()->setOrientation(1);
        else if (orientation == 3) // WEST
            element->getClient()->setOrientation(2);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
