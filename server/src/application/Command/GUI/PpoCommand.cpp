/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.cpp
*/

#include "application/Command/GUI/PpoCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "GuiCommandParser.hpp"
#include "application/Server/Server.hpp"

#include "ServerException.hpp"
#include <iostream>

void PpoCommand::Accept(IGuiVisitor *visitor) {
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("PpoCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_ppo([[maybe_unused]] std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        PpoCommand cmd;
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_ppo failed: ") + e.what());
    }
}

void GuiVisitor::visit([[maybe_unused]] PpoCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit: ") + e.what());
    }
}

void PpoCommand::setPlayerId(int id) {
    _playerId = id;
}

int PpoCommand::getPlayerId() const {
    return _playerId;
}
