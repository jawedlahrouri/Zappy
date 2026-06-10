/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SetCommand.cpp
*/

#include "Command/AI/SetCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void SetCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("SetCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_set([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] INetworkProvider &network)
{
    try {
SetCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_set failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] SetCommand *element) {
    try {
} catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
