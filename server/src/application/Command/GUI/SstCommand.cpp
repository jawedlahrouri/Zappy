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

void GuiVisitor::visit(SstCommand *element) {
    try {
        Server* server = dynamic_cast<Server*>(&_network);
        if (!server)
            return;

        int newFreq = element->getNewFreq();

        // ⚠️ NOTE : Ici, on suppose que Server possède un moyen de modifier sa fréquence.
        // Si tu n'as pas de setter public "setFreq", il faudra modifier Server.hpp 
        // pour ajouter par exemple une méthode de mise à jour, ou adapter cette ligne.
        // Exemple classique : server->setFreq(newFreq);
        // Pour l'instant, on applique la modification (à adapter selon tes attributs de Server.hpp)
        
        server->setFreq(newFreq)
        std::string response = "sgt " + std::to_string(newFreq);
        _network.sendResponse(element->getClient()->getId(), response);
    } catch (const std::exception &e) {
        throw ServerException(std::string("GuiVisitor::visit(SstCommand): ") + e.what());
    }
}
