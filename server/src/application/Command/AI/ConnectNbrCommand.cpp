/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectNbrCommand.cpp
*/

#include "application/Command/AI/ConnectNbrCommand.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "AiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void ConnectNbrCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("ConnectNbrCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_connect_nbr([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        ConnectNbrCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_connect_nbr failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] ConnectNbrCommand *element) {
    try {
        auto client = element->getClient();
        int count = 0;
        for (auto &other : _network.getClients()) {
            if (other.second->getType() == ClientType::AI) {
                auto aiOther = std::dynamic_pointer_cast<AI>(other.second);
                if (aiOther && aiOther->getTeamName() == client->getTeamName()) {
                    count++;
                }
            }
        }
        
        int slots = _network.getClientsNb() - count;
        if (slots < 0) slots = 0;
        
        _network.sendResponse(client->getId(), std::to_string(slots) + "\n");
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
