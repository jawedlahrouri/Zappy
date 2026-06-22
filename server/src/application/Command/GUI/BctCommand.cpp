/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BctCommand.cpp
*/
#include "application/Command/GUI/BctCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "domain/Map/Map.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

#include <iostream>
#include <sstream>

void BctCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("BctCommand::Accept: ") + e.what());
    }
}

void BctCommand::setClient(std::shared_ptr<GUI> client)
{
    _client = client;
}

std::shared_ptr<GUI> BctCommand::getClient() const
{
    return _client;
}

void BctCommand::setX(int x)
{
    _x = x;
}

void BctCommand::setY(int y)
{
    _y = y;
}

int BctCommand::getX() const
{
    return _x;
}
    
int BctCommand::getY() const
{
    return _y;
}

void GuiCommandParser::parse_gui_bct(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::istringstream iss(command);
        std::string cmd;
        int x = 0;
        int y = 0;

        if (!(iss >> cmd >> x >> y)) {
            network.sendResponse(client->getId(), "sbp");
            return;
        }
        BctCommand bctCmd;
        bctCmd.setX(x);
        bctCmd.setY(y);
        bctCmd.setClient(client);
        GuiVisitor visitor(network);
        bctCmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_bct failed: ") + e.what());
    }
}

void GuiVisitor::visit(BctCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server || !server->getGameEngine())
            return;

        auto engine = server->getGameEngine().get();
        int x = element->getX();
        int y = element->getY();

        if (x < 0 || x >= engine->getWidth() || y < 0 || y >= engine->getHeight()) {
            _network.sendResponse(element->getClient()->getId(), "sbp");
            return;
        }

        const Tile& tile = engine->getMap().getTile(x, y);
        std::string response = "bct " + std::to_string(x) + " " + std::to_string(y) + " " +
                            std::to_string(tile.resources[FOOD]) + " " +
                            std::to_string(tile.resources[LINEMATE]) + " " +
                            std::to_string(tile.resources[DERAUMERE]) + " " +
                            std::to_string(tile.resources[SIBUR]) + " " +
                            std::to_string(tile.resources[MENDIANE]) + " " +
                            std::to_string(tile.resources[PHIRAS]) + " " +
                            std::to_string(tile.resources[THYSTAME]);
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(BctCommand): ") + e.what());
    }
}
