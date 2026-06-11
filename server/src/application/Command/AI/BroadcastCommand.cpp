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

#include "ServerException.hpp"
#include <iostream>

void BroadcastCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("BroadcastCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_broadcast(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network)
{
    (void)command;
    try {
        BroadcastCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_broadcast failed: ") + e.what());
    }
}

void AiVisitor::visit(BroadcastCommand *element) {
    (void)element;
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
