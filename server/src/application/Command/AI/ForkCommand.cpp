/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForkCommand.cpp
*/

#include "application/Command/AI/ForkCommand.hpp"
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
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_fork failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] ForkCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
