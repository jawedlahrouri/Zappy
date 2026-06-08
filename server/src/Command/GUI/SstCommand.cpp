/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SstCommand.cpp
*/

#include "Command/GUI/SstCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void SstCommand::Accept(IGuiVisitor *visitor) {
    visitor->visit(this);
}

void GuiCommandParser::parse_gui_sst(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        SstCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_sst failed: ") + e.what());
    }
}

void GuiVisitor::visit(SstCommand *element) {
    (void)element;
}
