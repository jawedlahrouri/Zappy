/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EjectCommand.cpp
*/

#include "application/Command/AI/EjectCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "application/Event/GUI/PexEvent.hpp"

#include "ServerException.hpp"
#include <iostream>

void EjectCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("EjectCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_eject([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        EjectCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_eject failed: ") + e.what());
    }
}

static std::pair<int, int> getDirectionFromOrientation(int orientation) {
    if (orientation == 0) return {0, -1}; // North
    if (orientation == 1) return {1, 0};  // East
    if (orientation == 2) return {0, 1};  // South
    if (orientation == 3) return {-1, 0}; // West
    return {0, 0};
}

static int getPushDirection(int dx, int dy, int orientation) {
    int absoluteDir = 0;
    if (dx == 0 && dy < 0) absoluteDir = 1;
    else if (dx < 0 && dy < 0) absoluteDir = 2;
    else if (dx < 0 && dy == 0) absoluteDir = 3;
    else if (dx < 0 && dy > 0) absoluteDir = 4;
    else if (dx == 0 && dy > 0) absoluteDir = 5;
    else if (dx > 0 && dy > 0) absoluteDir = 6;
    else if (dx > 0 && dy == 0) absoluteDir = 7;
    else if (dx > 0 && dy < 0) absoluteDir = 8;
    return (absoluteDir - 1 + (orientation * 2)) % 8 + 1;
}

static void check_bound(int &x, int &y, int width, int height)
{
    if (x < 0) x = width - 1;
    if (x >= width) x = 0;
    if (y < 0) y = height - 1;
    if (y >= height) y = 0;
}

static bool destroy_egg(std::shared_ptr<AI> client, INetworkProvider &network) {
    auto tile = network.getGameEngine()->getTile(client->getX(), client->getY());
    auto eggs = tile.getEggs();
    bool destroyed = false;
    for (auto &egg : eggs) {
        network.getGameEngine()->removeEgg(egg->getId());
        network.sendToGuis("edi " + std::to_string(egg->getId()) + "\n");
        destroyed = true;
    }
    return destroyed;
}

void AiVisitor::visit([[maybe_unused]] EjectCommand *element) {
    try {
        auto client = element->getClient();
        
        auto dir = getDirectionFromOrientation(client->getOrientation());
        int x_dir = dir.first;
        int y_dir = dir.second;
        
        int width = _network.getGameEngine()->getWidth();
        int height = _network.getGameEngine()->getHeight();
        
        bool ejected_someone = false;
        
        auto pos = std::make_pair(client->getX(), client->getY());
        
        for (auto &other : _network.getClients()) {
            if (other.second->getId() == client->getId()) continue;
            if (other.second->getType() == ClientType::GUI) continue;
            
            auto aiOther = std::dynamic_pointer_cast<AI>(other.second);
            if (!aiOther) continue;
            
            auto otherPos = std::make_pair(aiOther->getX(), aiOther->getY());
            
            if (pos == otherPos) {
                ejected_someone = true;
                
                int old_x = aiOther->getX();
                int old_y = aiOther->getY();
                int new_x = old_x + x_dir;
                int new_y = old_y + y_dir;
                
                check_bound(new_x, new_y, width, height);
                
                aiOther->setX(new_x);
                aiOther->setY(new_y);
                
                int push_k = getPushDirection(-x_dir, -y_dir, aiOther->getOrientation());
                _network.sendResponse(aiOther->getId(), "eject: " + std::to_string(push_k) + "\n");
                
                _network.getGameEngine()->moveAi(aiOther->getId(), old_x, old_y, new_x, new_y);
            }
        }
        
        bool eggs_destroyed = destroy_egg(client, _network);
        
        if (ejected_someone || eggs_destroyed) {
            _network.sendResponse(client->getId(), "ok\n");
            if (ejected_someone) {
                _network.sendEvent(PexEvent(client->getId()));
            }
        } else {
            _network.sendResponse(client->getId(), "ko\n");
        }
        
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
