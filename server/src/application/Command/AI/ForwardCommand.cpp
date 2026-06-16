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
        auto client = element->getClient();
        int old_x = client->getX();
        int old_y = client->getY();
        int orientation = client->getOrientation();
        
        if (orientation == 1) // NORTH
            client->setY(old_y - 1);
        else if (orientation == 2) // EAST
            client->setX(old_x + 1);
        else if (orientation == 3) // SOUTH
            client->setY(old_y + 1);
        else if (orientation == 4) // WEST
            client->setX(old_x - 1);
            
        borderWrap(client, _network.getGameEngine()->getWidth(), _network.getGameEngine()->getHeight());
        
        _network.getGameEngine()->getMap().moveAi(client->getId(), old_x, old_y, client->getX(), client->getY());
        
        _network.sendResponse(client->getId(), "ok\n");
        
        std::string ppo_msg = "ppo " + std::to_string(client->getId()) + " " +
                            std::to_string(client->getX()) + " " +
                            std::to_string(client->getY()) + " " +
                            std::to_string(client->getOrientation()) + "\n";
        _network.sendToGuis(ppo_msg);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
