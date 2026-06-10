/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinCommand.cpp
*/

#include "Command/GUI/PinCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void PinCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("PinCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_pin([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] INetworkProvider &network)
{
    try {
PinCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_pin failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] PinCommand *element) {
    try {
} catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
