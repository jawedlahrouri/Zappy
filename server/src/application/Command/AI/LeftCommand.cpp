/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LeftCommand.cpp
*/

#include "Command/AI/LeftCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void LeftCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("LeftCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_left([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        LeftCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_left failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] LeftCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
