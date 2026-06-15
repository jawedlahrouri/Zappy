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
    (void)element;
    try {
        int orientation = element->getClient()->getOrientation();

        if (orientation == 0) // NORTH
            element->getClient()->setOrientation(1);
        else if (orientation == 1) // EAST
            element->getClient()->setOrientation(2);
        else if (orientation == 2) // SOUTH
            element->getClient()->setOrientation(3);
        else if (orientation == 3) // WEST
            element->getClient()->setOrientation(0);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
