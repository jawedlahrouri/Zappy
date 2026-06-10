/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForwardCommand.cpp
*/

#include "Command/AI/ForwardCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void ForwardCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("ForwardCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_forward([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] INetworkProvider &network)
{
    try {
ForwardCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_forward failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] ForwardCommand *element) {
    try {
} catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
