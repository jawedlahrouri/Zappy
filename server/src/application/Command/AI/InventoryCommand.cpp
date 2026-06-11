/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InventoryCommand.cpp
*/

#include "Command/AI/InventoryCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void InventoryCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("InventoryCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_inventory([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        InventoryCommand cmd;
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_inventory failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] InventoryCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
