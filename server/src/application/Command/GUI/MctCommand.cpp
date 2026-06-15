/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.cpp
*/
#include "application/Command/GUI/MctCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

#include <iostream>

void MctCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("MctCommand::Accept: ") + e.what());
    }
}

void MctCommand::setClient(std::shared_ptr<GUI> client)
{
    _client = client;
}

std::shared_ptr<GUI> MctCommand::getClient() const
{
    return _client;
}

void GuiCommandParser::parse_gui_mct(std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        MctCommand cmd;
        cmd.setClient(client);
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_mct failed: ") + e.what());
    }
}

void GuiVisitor::visit(MctCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server || !server->getGameEngine())
            return;

        auto engine = server->getGameEngine().get();
        int width = engine->getWidth();
        int height = engine->getHeight();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Tile& tile = engine->getTile(x, y);
                std::string response = "bct " + std::to_string(x) + " " + std::to_string(y) + " " +
                                       std::to_string(tile.resources[FOOD]) + " " +
                                       std::to_string(tile.resources[LINEMATE]) + " " +
                                       std::to_string(tile.resources[DERAUMERE]) + " " +
                                       std::to_string(tile.resources[SIBUR]) + " " +
                                       std::to_string(tile.resources[MENDIANE]) + " " +
                                       std::to_string(tile.resources[PHIRAS]) + " " +
                                       std::to_string(tile.resources[THYSTAME]);
                                       
                _network.sendResponse(element->getClient()->getId(), response);
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(MctCommand): ") + e.what());
    }
}