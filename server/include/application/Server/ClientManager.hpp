/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ClientManager.hpp
*/

/**
 * @file ClientManager.hpp
 * @brief Registry of connected clients and helper to message them.
 */

#ifndef CLIENTMANAGER_HPP_
#define CLIENTMANAGER_HPP_

#include <unordered_map>
#include <memory>
#include <string>
#include "Client.hpp"

class INetworkProvider;
class ILogger;

/**
 * @class ClientManager
 * @brief Owns connected clients (keyed by fd) and queues outgoing messages.
 *
 * Centralises add/remove/lookup of clients and the send helpers that buffer a
 * message and arm @c POLLOUT on the right fd(s).
 */
class ClientManager {
    public:
        /**
         * @brief Builds the manager.
         * @param network Network facade used when sending/removing clients.
         * @param logger Logger for connection events.
         */
        ClientManager(INetworkProvider& network, ILogger& logger);
        ~ClientManager() = default;

        /** @brief Registers a client under its fd. */
        void addClient(int fd, std::shared_ptr<Client> client);
        /** @brief Flushes, closes and unregisters the client on @p fd. */
        void removeClient(int fd);
        /** @brief Returns the client on @p fd, or @c nullptr if unknown. */
        std::shared_ptr<Client> getClient(int fd);
        /** @brief Returns all clients keyed by fd. */
        const std::unordered_map<int, std::shared_ptr<Client>>& getClients() const;

        /** @brief Queues a message to one client (newline-terminated). */
        void sendResponse(int clientId, const std::string& message);
        /** @brief Queues a message to every GUI client. */
        void sendToGuis(const std::string& message);
        /** @brief Queues a message to every AI client. */
        void sendToAi(const std::string& message);
        /** @brief Queues a message to all clients except @p excludeClientId. */
        void broadcastEvent(const std::string& message, int excludeClientId = -1);

        /** @brief Disconnects and clears every client. */
        void disconnectAll();

    private:
        std::unordered_map<int, std::shared_ptr<Client>> _clients; /**< By fd. */
        INetworkProvider& _network; /**< Network facade. */
        ILogger& _logger;           /**< Connection-event logger. */
};

#endif /* !CLIENTMANAGER_HPP_ */
