/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LookCommand.cpp
*/

#include "application/Command/AI/LookCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/GameEngine/GameEngine.hpp"

#include "ServerException.hpp"
#include <iostream>

void LookCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("LookCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_look([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        LookCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_look failed: ") + e.what());
    }
}

static std::pair<int, int> getTileCoordinates(int base_x, int base_y, int orientation, int d, int w) {
    if (orientation == 1) return {base_x + w, base_y - d}; // NORTH
    if (orientation == 2) return {base_x + d, base_y + w}; // EAST
    if (orientation == 3) return {base_x - w, base_y + d}; // SOUTH
    return {base_x - d, base_y - w}; // WEST
}

static std::string getTileContentString(const Tile& tile) {
    static const std::array<std::string, RESOURCE_COUNT> resourceNames = {
        "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
    };
    std::string content = "";

    for (size_t p = 0; p < tile.playerIds.size(); p++) {
        content += "player ";
    }
    for (int r = 0; r < RESOURCE_COUNT; r++) {
        for (int c = 0; c < tile.resources[r]; c++) {
            content += resourceNames[r] + " ";
        }
    }
    if (!content.empty() && content.back() == ' ') {
        content.pop_back();
    }
    return content;
}

static std::string buildLookOutput(std::shared_ptr<AI> client, GameEngine* engine) {
    int pos_x = client->getX();
    int pos_y = client->getY();
    int orientation = client->getOrientation();
    int level = client->getLevel();
    std::string output = "[";

    for (int d = 0; d <= level; d++) {
        for (int w = -d; w <= d; w++) {
            auto [tile_x, tile_y] = getTileCoordinates(pos_x, pos_y, orientation, d, w);
            auto tile = engine->getMap().getTile(tile_x, tile_y);
            output += getTileContentString(tile);
            if (d != level || w != d) {
                output += ",";
            }
        }
    }
    output += "]\n";
    return output;
}

void AiVisitor::visit([[maybe_unused]] LookCommand *element) {
    try {
        auto client = element->getClient();
        auto& engine = _network.getGameEngine();
        
        std::string output = buildLookOutput(client, engine.get());
        _network.sendResponse(client->getId(), output);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
