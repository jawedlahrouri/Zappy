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
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_look failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] LookCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
