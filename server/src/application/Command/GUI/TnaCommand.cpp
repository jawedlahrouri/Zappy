/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TnaCommand.cpp
*/
#include "application/Command/GUI/TnaCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

void TnaCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("TnaCommand::Accept: ") + e.what());
    }
}

void TnaCommand::setClient(std::shared_ptr<GUI> client)
{
    _client = client;
}

std::shared_ptr<GUI> TnaCommand::getClient() const
{
    return _client;
}

void GuiCommandParser::parse_gui_tna(std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        TnaCommand cmd;
        cmd.setClient(client);
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_tna failed: ") + e.what());
    }
}

void GuiVisitor::visit(TnaCommand *element)
{
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server)
            return;

        for (const auto& teamName : server->getTeamNames()) {
            _network.sendResponse(element->getClient()->getId(), "tna " + teamName);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(TnaCommand): ") + e.what());
    }
}
