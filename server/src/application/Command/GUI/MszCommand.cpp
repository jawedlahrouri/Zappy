/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MszCommand.cpp
*/
#include "application/Command/GUI/MszCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "ServerException.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "domain/Client/GUI.hpp"

void MszCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("MszCommand::Accept: ") + e.what());
    }
}

void MszCommand::setClient(std::shared_ptr<GUI> client)
{
    _client = client;
}

std::shared_ptr<GUI> MszCommand::getClient() const
{
    return _client;
}

void GuiCommandParser::parse_gui_msz(std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        MszCommand cmd;
        cmd.setClient(client);
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_msz failed: ") + e.what());
    }
}

void GuiVisitor::visit(MszCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server || !server->getGameEngine())
            return;

        int width = server->getGameEngine()->getWidth();
        int height = server->getGameEngine()->getHeight();

        std::string response = "msz " + std::to_string(width) + " " + std::to_string(height);
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(MszCommand): ") + e.what());
    }
}
