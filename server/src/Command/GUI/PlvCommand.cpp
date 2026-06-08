/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlvCommand.cpp
*/

#include "Command/GUI/PlvCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PlvCommand::Accept(IGuiVisitor *visitor) {
    visitor->visit(this);
}

void GuiCommandParser::parse_gui_plv(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        PlvCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_plv failed: ") + e.what());
    }
}

void GuiVisitor::visit(PlvCommand *element) {
    (void)element;
}
