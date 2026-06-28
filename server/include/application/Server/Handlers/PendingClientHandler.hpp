/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PendingClientHandler.hpp
*/

#ifndef PENDINGCLIENTHANDLER_HPP_
#define PENDINGCLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

/**
 * @class PendingClientHandler
 * @brief Strategy handling input from a not-yet-identified client.
 *
 * Concrete @ref IClientDataHandler for @c PENDING clients: reads the first line
 * and resolves the client into an AI or GUI (see
 * @ref ConnectionHandler::detectClientType).
 */
class PendingClientHandler : public IClientDataHandler {
    public:
        PendingClientHandler() = default;
        ~PendingClientHandler() override = default;

        /** @brief Resolves the client type from its first handshake line. */
        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !PENDINGCLIENTHANDLER_HPP_ */
