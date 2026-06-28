/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SstCommand.cpp
*/
#include "application/Command/GUI/SstCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "presentation/Parsing/GuiCommandParser.hpp"
#include "application/Server/Server.hpp"
#include "domain/Client/GUI.hpp"
#include "shared/ServerException.hpp"

#include <iostream>
#include <sstream>

void SstCommand::Accept(IGuiVisitor *visitor)
{
    try {
        visitor->visit(this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("SstCommand::Accept: ") + e.what());
    }
}

void SstCommand::setNewFreq(int freq)
{
    _newFreq = freq;
}

int SstCommand::getNewFreq() const
{
    return _newFreq;
}

void GuiCommandParser::parse_gui_sst(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::istringstream iss(command);
        std::string cmd;
        int newFreq = 0;

        if (!(iss >> cmd >> newFreq) || newFreq <= 0) {
            network.sendResponse(client->getId(), "sbp");
            return;
        }
        SstCommand sstCmd;
        sstCmd.setNewFreq(newFreq);
        sstCmd.setClient(client);
        GuiVisitor visitor(network);
        sstCmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_gui_sst failed: ") + e.what());
    }
}

/**
 * @brief Applies the new time unit and echoes it back.
 * @par Protocol (GUI protocol)
 * Request "sst T\n" -> Response "sgt T\n" (an invalid value yields "sbp").
 */
void GuiVisitor::visit(SstCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server)
            return;

        int newFreq = element->getNewFreq();
        
        server->setFreq(newFreq);
        std::string response = "sgt " + std::to_string(newFreq);
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(SstCommand): ") + e.what());
    }
}
