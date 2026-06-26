/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SgtCommand.cpp
*/
#include "application/Command/GUI/SgtCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

void SgtCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("SgtCommand::Accept: ") + e.what());
    }
}

void GuiCommandParser::parse_gui_sgt(std::shared_ptr<GUI> client, [[maybe_unused]] const std::string &command, INetworkProvider &network)
{
    try {
        SgtCommand cmd;
        cmd.setClient(client);
        GuiVisitor visitor(network);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_sgt failed: ") + e.what());
    }
}

/**
 * @brief Replies with the current time unit (frequency).
 * @par Protocol (GUI protocol)
 * Request "sgt\n" -> Response "sgt T\n".
 */
void GuiVisitor::visit(SgtCommand *element)
{
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server)
            return;

        std::string response = "sgt " + std::to_string(server->getFreq());
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(SgtCommand): ") + e.what());
    }
}
