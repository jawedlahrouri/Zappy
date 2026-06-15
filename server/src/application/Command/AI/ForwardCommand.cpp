/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForwardCommand.cpp
*/

#include "application/Command/AI/ForwardCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>
#include <unordered_map>
#include "domain/GameEngine/GameEngine.hpp"

static void borderWrap(std::shared_ptr<AI> client, int width, int height) {
    int x = client->getX();
    int y = client->getY();

    if (x < 0)
        client->setX(width - 1);
    else if (x >= width)
        client->setX(0);
    else if (y < 0)
        client->setY(height - 1);
    else if (y >= height)
        client->setY(0);
}

void ForwardCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("ForwardCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_forward([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        ForwardCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_forward failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] ForwardCommand *element) {
    try {
        int orientation = element->getClient()->getOrientation();
        
        if (orientation == 0) // NORTH
            element->getClient()->setY(element->getClient()->getY() + 1);
        else if (orientation == 1) // EAST
            element->getClient()->setX(element->getClient()->getX() + 1);
        else if (orientation == 2) // SOUTH
            element->getClient()->setY(element->getClient()->getY() - 1);
        else if (orientation == 3) // WEST
            element->getClient()->setX(element->getClient()->getX() - 1);
        borderWrap(element->getClient(), _network.getGameEngine()->getWidth(), _network.getGameEngine()->getHeight()); // teleport to the other side if the player goes out of bounds
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
