/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TnaCommand.cpp
*/

#include "Command/GUI/TnaCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void TnaCommand::Accept(IGuiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("TnaCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_tna([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        TnaCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_tna failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] TnaCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
