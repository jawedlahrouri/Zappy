/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectionHandler.hpp
*/

#ifndef CONNECTIONHANDLER_HPP_
#define CONNECTIONHANDLER_HPP_

#include <vector>
#include <string>

class ISocket;
class IPoll;
class ClientManager;
class INetworkProvider;
class ILogger;

class ConnectionHandler {
    public:
        ConnectionHandler() = default;
        ~ConnectionHandler() = default;

        static void acceptClient(ISocket& masterSocket, IPoll& poll, ClientManager& clientManager, ILogger& logger);
        static void detectClientType(int clientSocket, ClientManager& clientManager, INetworkProvider& network, ILogger& logger, const std::vector<std::string>& teamNames);
};

#endif /* !CONNECTIONHANDLER_HPP_ */
