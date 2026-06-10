/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BroadcastCommand.cpp
*/

#include "Command/AI/BroadcastCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void BroadcastCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("BroadcastCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_broadcast([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] INetworkProvider &network)
{
    try {
BroadcastCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_broadcast failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] BroadcastCommand *element) {
    try {
} catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
