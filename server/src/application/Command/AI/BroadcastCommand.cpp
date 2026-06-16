/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BroadcastCommand.cpp
*/

#include "application/Command/AI/BroadcastCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "ServerException.hpp"
#include <iostream>

void BroadcastCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("BroadcastCommand::Accept: ") + e.what());
    }
}

static std::pair<int, int> getTileDistance(const std::pair<int, int> &pos, const std::pair<int, int> &otherPos, INetworkProvider &_network)
{
    int dx = otherPos.first - pos.first;
    int dy = otherPos.second - pos.second;
    
    int mapWidth = _network.getGameEngine()->getWidth();
    int mapHeight = _network.getGameEngine()->getHeight();
    
    if (dx > mapWidth / 2) dx -= mapWidth;
    else if (dx < -mapWidth / 2) dx += mapWidth;
    if (dy > mapHeight / 2) dy -= mapHeight;
    else if (dy < -mapHeight / 2) dy += mapHeight;
    
    return std::make_pair(dx, dy);
}

static int getSoundDirection(int dx, int dy, int orientation)
{
    if (dx == 0 && dy == 0) return 0;
    
    int absoluteDir = 0;
    if (dx == 0 && dy < 0) absoluteDir = 1;
    else if (dx < 0 && dy < 0) absoluteDir = 2;
    else if (dx < 0 && dy == 0) absoluteDir = 3;
    else if (dx < 0 && dy > 0) absoluteDir = 4;
    else if (dx == 0 && dy > 0) absoluteDir = 5;
    else if (dx > 0 && dy > 0) absoluteDir = 6;
    else if (dx > 0 && dy == 0) absoluteDir = 7;
    else if (dx > 0 && dy < 0) absoluteDir = 8;
    
    return (absoluteDir - 1 + ((orientation - 1) * 2)) % 8 + 1;
}

void AiCommandParser::parse_broadcast(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network)
{
    try {
        BroadcastCommand cmd;

        size_t space_pos = command.find(' ');
        std::string message = "";
        if (space_pos != std::string::npos) {
            message = command.substr(space_pos + 1);
            if (!message.empty() && message.back() == '\n')
                message.pop_back();
            if (!message.empty() && message.back() == '\r')
                message.pop_back();
        }

        cmd.setMessage(message);
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_broadcast failed: ") + e.what());
    }
}

void AiVisitor::visit(BroadcastCommand *element) {
    try {
        auto senderPos = std::make_pair(element->getClient()->getX(), element->getClient()->getY());
        
        for (auto &other : _network.getClients()) {
            if (other.second->getId() == element->getClient()->getId()) continue;
            if (other.second->getType() == ClientType::GUI) continue;
            
            auto receiverClient = std::dynamic_pointer_cast<AI>(other.second);
            if (!receiverClient) continue;
            
            auto receiverPos = std::make_pair(receiverClient->getX(), receiverClient->getY());
            auto dist = getTileDistance(receiverPos, senderPos, _network);

            int direction = getSoundDirection(dist.first, dist.second, receiverClient->getOrientation());
            std::string messages = "message " + std::to_string(direction) + ", " + element->getMessage() + "\n";
            _network.sendResponse(other.second->getId(), messages);
        }
        _network.sendResponse(element->getClient()->getId(), "ok\n");
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}

void BroadcastCommand::setMessage(const std::string& message) {
    _message = message;
}

std::string BroadcastCommand::getMessage() const {
    return _message;
}
