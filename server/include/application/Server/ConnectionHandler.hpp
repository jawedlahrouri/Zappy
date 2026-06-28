/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectionHandler.hpp
*/

/**
 * @file ConnectionHandler.hpp
 * @brief Stateless helpers for accepting clients and resolving their type.
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

/**
 * @class ConnectionHandler
 * @brief Static utilities for the connection lifecycle (accept + handshake).
 */
class ConnectionHandler {
    public:
        ConnectionHandler() = default;
        ~ConnectionHandler() = default;

        /**
         * @brief Accepts a new connection, registers it and greets it.
         * @param masterSocket Listening socket to accept from.
         * @param poll Multiplexer to watch the new fd for input.
         * @param clientManager Registry to store the new client.
         * @param logger Logger for the connection event.
         * @details Sets the client socket non-blocking + TCP_NODELAY, adds it to
         *          @p poll, registers a @c PENDING client and sends @c "WELCOME".
         */
        static void acceptClient(ISocket& masterSocket, IPoll& poll, ClientManager& clientManager, ILogger& logger);

        /**
         * @brief Resolves a pending client into an AI or GUI from its first line.
         * @param clientSocket Client fd.
         * @param clientManager Client registry.
         * @param network Network facade for replies/login.
         * @param logger Logger.
         * @param teamNames Valid team names (a line equal to "GRAPHIC" means GUI).
         */
        static void detectClientType(int clientSocket, ClientManager& clientManager, INetworkProvider& network, ILogger& logger, const std::vector<std::string>& teamNames);
};

#endif /* !CONNECTIONHANDLER_HPP_ */
