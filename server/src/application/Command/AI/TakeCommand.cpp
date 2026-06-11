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
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_take failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] TakeCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
