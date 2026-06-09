/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PfcCommand.cpp
*/

#include "Command/GUI/PfcCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PfcCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("PfcCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_pfc(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        PfcCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_pfc failed: ") + e.what());
    }
}

void GuiVisitor::visit(PfcCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GuiVisitor::visit: ") + e.what());
    }
}
