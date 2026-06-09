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
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void SgtCommand::Accept(IGuiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("SgtCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_sgt([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] Server &server)
{
    try {
        SgtCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_sgt failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] SgtCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("GuiVisitor::visit: ") + e.what());
    }
}
