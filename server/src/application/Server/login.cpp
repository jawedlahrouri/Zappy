/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.cpp
*/

#include "ServerException.hpp"
#include "application/Server/Server.hpp"
#include "application/Server/login.hpp"
#include "GameEngine.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "application/Event/GUI/PnwEvent.hpp"
#include "AI.hpp"
#include "GUI.hpp"

// protocol:
// <-- WELCOME (sent on connection accept)
// --> TEAM-NAME
// <-- CLIENT-NUM (unused slots)
// <-- X Y (map dimensions)
void Login::login_ai(std::shared_ptr<AI> ai, const std::string &team_name, INetworkProvider &network)
{
    try {
    int count = 0;
    for (const auto &[fd_client, client] : network.getClients()) {
        if (client->getType() == ClientType::AI) {
            auto ai_client = std::dynamic_pointer_cast<AI>(client);
            if (ai_client && ai_client->getTeamName() == team_name) {
                count++;
            }
        }
    }
    int slots = network.getClientsNb() - count;

    int fd = ai->getId();
    network.sendResponse(fd, std::to_string(slots));
    if (network.getGameEngine()) {
        network.sendResponse(fd, std::to_string(network.getGameEngine()->getWidth()) + " " + std::to_string(network.getGameEngine()->getHeight()));
        network.getGameEngine()->addAiToTile(ai->getId(), ai->getX(), ai->getY());
    } else {
        network.sendResponse(fd, "0 0");
    }
    
    network.sendEvent(PnwEvent(ai));
    } catch (const std::exception &e) {
        throw ServerException(std::string("Login::login_ai: ") + e.what());
    }
}

// protocol:
// <-- WELCOME (sent on connection accept)
// --> GRAPHIC
// <-- msz X Y\n
// <-- sgt T\n
// <-- tna N\n * nbr_teams
void Login::login_gui(std::shared_ptr<GUI> gui, INetworkProvider &network)
{
    try {
    int fd = gui->getId();

    if (network.getGameEngine()) {
        network.sendResponse(fd, "msz " + std::to_string(network.getGameEngine()->getWidth()) + " " + std::to_string(network.getGameEngine()->getHeight()));
    } else {
        network.sendResponse(fd, "msz 0 0");
    }
    network.sendResponse(fd, "sgt " + std::to_string(network.getFreq()));
    for (const auto &team : network.getTeamNames()) {
        network.sendResponse(fd, "tna " + team);
    }
    for (const auto &[client_fd, client] : network.getClients()) {
        if (client->getType() == ClientType::AI) {
            auto ai = std::dynamic_pointer_cast<AI>(client);
            if (ai) {
                std::string pnw_msg = "pnw " + std::to_string(ai->getId()) + " " +
                                    std::to_string(ai->getX()) + " " + std::to_string(ai->getY()) + " " +
                                    std::to_string(ai->getOrientation()) + " " +
                                    std::to_string(ai->getLevel()) + " " + ai->getTeamName();
                network.sendResponse(fd, pnw_msg);
            }
        }
    }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Login::login_gui: ") + e.what());
    }
}
