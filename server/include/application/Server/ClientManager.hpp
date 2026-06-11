/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ClientManager.hpp
*/

#ifndef CLIENTMANAGER_HPP_
#define CLIENTMANAGER_HPP_

#include <unordered_map>
#include <memory>
#include <string>
#include "Client/Client.hpp"

class INetworkProvider;
class ILogger;

class ClientManager {
    public:
        ClientManager(INetworkProvider& network, ILogger& logger);
        ~ClientManager() = default;

        void addClient(int fd, std::shared_ptr<Client> client);
        void removeClient(int fd);
        std::shared_ptr<Client> getClient(int fd);
        const std::unordered_map<int, std::shared_ptr<Client>>& getClients() const;

        void sendResponse(int clientId, const std::string& message);
        void sendToGuis(const std::string& message);
        void sendToAi(const std::string& message);
        void broadcastEvent(const std::string& message, int excludeClientId = -1);

        void disconnectAll();

    private:
        std::unordered_map<int, std::shared_ptr<Client>> _clients;
        INetworkProvider& _network;
        ILogger& _logger;
};

#endif /* !CLIENTMANAGER_HPP_ */
