/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.cpp
*/

#include "application/Command/GUI/MctCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "GuiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void MctCommand::Accept(IGuiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("MctCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_mct([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        MctCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_mct failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] MctCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
