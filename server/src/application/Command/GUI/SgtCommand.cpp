/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SgtCommand.cpp
*/

#include "Command/GUI/SgtCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void SgtCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("SgtCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_sgt(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        (void)client;
        (void)command;
        (void)network;
        SgtCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_sgt failed: ") + e.what());
    }
}

void GuiVisitor::visit(SgtCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
