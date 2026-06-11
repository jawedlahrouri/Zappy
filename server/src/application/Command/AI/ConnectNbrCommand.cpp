/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectNbrCommand.cpp
*/

#include "Command/AI/ConnectNbrCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void ConnectNbrCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("ConnectNbrCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_connect_nbr([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        ConnectNbrCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_connect_nbr failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] ConnectNbrCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
