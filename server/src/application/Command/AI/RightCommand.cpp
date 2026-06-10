/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.cpp
*/

#include "Command/AI/RightCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void RightCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("RightCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_right([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] INetworkProvider &network)
{
    try {
RightCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_right failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] RightCommand *element) {
    try {
} catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
