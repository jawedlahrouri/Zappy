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

#include "ServerException.hpp"
#include <iostream>

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

void AiVisitor::visit([[maybe_unused]] IncantationCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
