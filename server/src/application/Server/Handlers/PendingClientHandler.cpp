/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PendingClientHandler.cpp
*/

#include "application/Server/Handlers/PendingClientHandler.hpp"
#include "application/Server/Server.hpp"
#include "application/Server/ConnectionHandler.hpp"
#include "Client.hpp"

void PendingClientHandler::handleData(std::shared_ptr<Client> client, Server& server)
{
    ConnectionHandler::detectClientType(client->getId(), server.getClientManager(), server, server.getLogger(), server.getTeamNames());
}
