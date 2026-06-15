/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForkCommand.cpp
*/

#include "application/Command/AI/ForkCommand.hpp"
#include "GameEngine.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void ForkCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("ForkCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_fork([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        ForkCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_fork failed: ") + e.what());
    }
}

#include "domain/Client/AI.hpp"

void AiVisitor::visit(ForkCommand *element) {
    try {
        auto aiClient = std::dynamic_pointer_cast<AI>(element->getClient());
        if (!aiClient)
            throw ServerException("Client is not an AI");
            
        int pos_x = aiClient->getX();
        int pos_y = aiClient->getY();
        
        int eggId = _network.getGameEngine()->generateEggId();
        
        auto egg = std::make_shared<Egg>();
        egg->id = eggId;
        egg->ownerId = aiClient->getId();
        egg->teamName = aiClient->getTeamName();
        egg->x = pos_x;
        egg->y = pos_y;
        
        _network.getGameEngine()->addEgg(egg);
        
        std::string enw_msg = "enw " + std::to_string(eggId) + " " + std::to_string(aiClient->getId()) + " " + std::to_string(pos_x) + " " + std::to_string(pos_y) + "\n";
        _network.sendToGuis(enw_msg);
        
        _network.sendResponse(aiClient->getId(), "ok\n");
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
