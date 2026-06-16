/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TakeCommand.cpp
*/

#include "application/Command/AI/TakeCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "ServerException.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include <iostream>

void TakeCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("TakeCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_take([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        TakeCommand cmd;
        cmd.setClient(client);
        
        size_t space_pos = command.find(' ');
        std::string resource_name = "";
        if (space_pos != std::string::npos) {
            resource_name = command.substr(space_pos + 1);
            if (!resource_name.empty() && resource_name.back() == '\n')
                resource_name.pop_back();
            if (!resource_name.empty() && resource_name.back() == '\r')
                resource_name.pop_back();
        }
        cmd.setResourceName(resource_name);

        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_set failed: ") + e.what());
    }
}

static ResourceType stringToResource(const std::string& name) {
    if (name == "food") return FOOD;
    if (name == "linemate") return LINEMATE;
    if (name == "deraumere") return DERAUMERE;
    if (name == "sibur") return SIBUR;
    if (name == "mendiane") return MENDIANE;
    if (name == "phiras") return PHIRAS;
    if (name == "thystame") return THYSTAME;
    return RESOURCE_COUNT;
}

void AiVisitor::visit([[maybe_unused]] TakeCommand *element) {
    try {
        std::string resource_name = element->getResourceName();
        ResourceType resType = stringToResource(resource_name);
        
        if (resType == RESOURCE_COUNT) {
            _network.sendResponse(element->getClient()->getId(), "ko\n");
            return;
        }

        auto& tile = _network.getGameEngine()->getMap().getTile(element->getClient()->getX(), element->getClient()->getY());
        
        if (tile.resources[resType] <= 0) {
            _network.sendResponse(element->getClient()->getId(), "ko\n");
            return;
        }
        
        _network.getGameEngine()->getMap().removeResourceFromTile(element->getClient()->getX(), element->getClient()->getY(), resType, 1);
        auto& inventory = element->getClient()->getInventory();
        inventory[resource_name]++;
        
        _network.sendResponse(element->getClient()->getId(), "ok\n");

        std::string pgt_msg = "pgt " + std::to_string(element->getClient()->getId()) + " " + std::to_string(resType) + "\n";
        _network.sendToGuis(pgt_msg);

        auto ai = element->getClient();
        auto& inv = ai->getInventory();
        std::string pin_msg = "pin " + std::to_string(ai->getId()) + " " +
                            std::to_string(ai->getX()) + " " +
                            std::to_string(ai->getY()) + " " +
                            std::to_string(inv.at("food")) + " " +
                            std::to_string(inv.at("linemate")) + " " +
                            std::to_string(inv.at("deraumere")) + " " +
                            std::to_string(inv.at("sibur")) + " " +
                            std::to_string(inv.at("mendiane")) + " " +
                            std::to_string(inv.at("phiras")) + " " +
                            std::to_string(inv.at("thystame")) + "\n";
        _network.sendToGuis(pin_msg);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}

void TakeCommand::setResourceName(const std::string& name) {
    _resourceName = name;
}

std::string TakeCommand::getResourceName() const {
    return _resourceName;
}
