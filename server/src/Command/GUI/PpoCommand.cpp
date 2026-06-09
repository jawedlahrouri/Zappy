/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.cpp
*/

#include "Command/GUI/PpoCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PpoCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("PpoCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_ppo(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        PpoCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_ppo failed: ") + e.what());
    }
}

void GuiVisitor::visit(PpoCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GuiVisitor::visit: ") + e.what());
    }
}
