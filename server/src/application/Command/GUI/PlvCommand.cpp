/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlvCommand.cpp
*/

#include "application/Command/GUI/PlvCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "GuiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void PlvCommand::Accept(IGuiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("PlvCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_plv([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        PlvCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_plv failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] PlvCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
