/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IncantationCommand.cpp
*/

#include "application/Command/AI/IncantationCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/AI.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "ServerException.hpp"

void IncantationCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("IncantationCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_incantation([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        IncantationCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_incantation failed: ") + e.what());
    }
}


void AiVisitor::visit(IncantationCommand *element) {
    try {
        auto aiClient = element->getClient();
        if (!aiClient)
            throw ServerException("Client is not an AI");

        int level = aiClient->getLevel();
        int x = aiClient->getX();
        int y = aiClient->getY();

        auto& engine = _network.getGameEngine();
        auto players = engine->getPlayersOnTileForIncantation(x, y, level, _network);

        if (players.size() > 0 && engine->checkIncantationRequirements(x, y, level)) {
            engine->consumeIncantationResources(x, y, level);
            for (auto& p : players) {
                p->setLevel(level + 1);
                _network.sendResponse(p->getId(), "Current level: " + std::to_string(level + 1) + "\n");   
                if (p->getLevel() == 8) {
                    engine->checkVictory(p->getTeamName(), _network);
                }
            }
            _network.sendToGuis("pie " + std::to_string(x) + " " + std::to_string(y) + " 1\n");
        } else {
            _network.sendResponse(aiClient->getId(), "ko\n");
            _network.sendToGuis("pie " + std::to_string(x) + " " + std::to_string(y) + " 0\n");
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
