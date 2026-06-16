/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InventoryCommand.cpp
*/

#include "application/Command/AI/InventoryCommand.hpp"
#include "AiCommandParser.hpp"
#include "application/Command/AI/AiVisitor.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void InventoryCommand::Accept(IAiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("InventoryCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_inventory(
    [[maybe_unused]] std::shared_ptr<AI> client,
    [[maybe_unused]] const std::string &command, INetworkProvider &network) {
    try {
        InventoryCommand cmd;
        cmd.setClient(client);
        AiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_inventory failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] InventoryCommand *element) {
    try {
        auto& inventory = element->getClient()->getInventory();
        std::vector<std::string> resources = {
            "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
        };
        std::string output = "[";

        for (size_t i = 0; i < resources.size(); ++i) {
            std::string res = resources[i];
            int qty = inventory.count(res) ? inventory.at(res) : 0;
            output += res + " " + std::to_string(qty);
            if (i < resources.size() - 1) {
                output += ", ";
            }
        }
        output += "]\n";
        _network.sendResponse(element->getClient()->getId(), output);
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiVisitor::visit: ") + e.what());
    }
}
