/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PicCommand.cpp
*/

#include "Command/GUI/PicCommand.hpp"
#include "Command/GUI/GuiVisitor.hpp"
#include "Parsing/GuiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void PicCommand::Accept(IGuiVisitor *visitor) {
    visitor->visit(this);
}

void GuiCommandParser::parse_gui_pic(std::shared_ptr<GUI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        PicCommand cmd;
        GuiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_gui_pic failed: ") + e.what());
    }
}

void GuiVisitor::visit(PicCommand *element) {
    (void)element;
}
