/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.cpp
*/

#include "Command/GUI/MctCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void MctCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("MctCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_mct(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        MctCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_mct failed: ") + e.what());
    }
}

void GuiVisitor::visit(MctCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GuiVisitor::visit: ") + e.what());
    }
}
