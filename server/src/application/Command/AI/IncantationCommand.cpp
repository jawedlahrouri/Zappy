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
#include "application/Event/GUI/PieEvent.hpp"
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
        cmd.setClient(client);
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
        int requiredPlayers = engine->getRequiredPlayersForIncantation(level);
        bool enoughPlayers = players.size() >= static_cast<size_t>(requiredPlayers);
        _network.getLogger().debug("IncantationExecute AI#" + std::to_string(aiClient->getId())
            + " lvl=" + std::to_string(level)
            + " pos=(" + std::to_string(x) + "," + std::to_string(y) + ")"
            + " players=" + std::to_string(players.size()) + "/" + std::to_string(requiredPlayers)
            + " enoughPlayers=" + std::to_string(enoughPlayers));

        if (enoughPlayers) {
            // Ressources déjà consommées au START — monter les niveaux
            for (auto& p : players) {
                p->setInIncantation(false);
                p->setLevel(level + 1);
                _network.getLogger().debug("IncantationSuccess: 'Current level:" + std::to_string(level + 1) + "' to fd=" + std::to_string(p->getId()));
                _network.sendResponse(p->getId(), "Current level: " + std::to_string(level + 1) + "\n");
                _network.sendToGuis("plv " + std::to_string(p->getId()) + " " + std::to_string(level + 1));
                if (p->getLevel() == 8) {
                    engine->checkVictory(p->getTeamName(), _network);
                }
            }
            _network.sendEvent(PieEvent(x, y, 1));
        } else {
            // Joueurs partis pendant le rituel : rembourser les ressources
            engine->refundIncantationResources(x, y, level);
            _network.sendResponse(aiClient->getId(), "ko\n");
            _network.sendEvent(PieEvent(x, y, 0));
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
