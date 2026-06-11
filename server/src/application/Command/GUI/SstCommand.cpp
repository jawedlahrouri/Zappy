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

#include "ServerException.hpp"
#include <iostream>

void SstCommand::Accept(IGuiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("SstCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_sst([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        SstCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_sst failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] SstCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}
