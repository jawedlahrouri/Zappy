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
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PinCommand::Accept(IGuiVisitor *visitor) {
    visitor->visit(this);
}

void GuiCommandParser::parse_gui_pin(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        PinCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_pin failed: ") + e.what());
    }
}

void GuiVisitor::visit(PinCommand *element) {
    (void)element;
}
