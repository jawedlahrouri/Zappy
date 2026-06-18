#include <memory>
class GameEngine;
/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** INetworkProvider.hpp
*/

#ifndef INETWORK_PROVIDER_HPP_
#define INETWORK_PROVIDER_HPP_

#include <string>
#include <vector>
#include <unordered_map>

#include "IPoll.hpp"

class INetworkProvider {
    public:
        virtual ~INetworkProvider() = default;

        virtual int getClientsNb() const = 0;
        virtual void setClientsNb(int nb) = 0;
        virtual int getFreq() const = 0;
        virtual const std::vector<std::string>& getTeamNames() const = 0;
        virtual const std::unique_ptr<GameEngine>& getGameEngine() const = 0;
        virtual void sendEvent(const class IEventElement& event) = 0;
        virtual const std::unordered_map<int, std::shared_ptr<class Client>>& getClients() const = 0;
        virtual class ILogger& getLogger() const = 0;
        virtual void addPollEvent(int fd, NetworkEvent event) = 0;
    

        virtual void sendResponse(int clientId, const std::string& message) = 0;
        virtual void sendToGuis(const std::string& message) = 0;
        virtual void broadcastEvent(const std::string& message, int excludeClientId = -1) = 0;
        virtual void executeCommand(std::shared_ptr<class AI> ai, const std::string& command) = 0;
        virtual void notifyVictory(const std::string& teamName) = 0;
        virtual int getCommandDuration(const std::string& command) const = 0;
        virtual void removeClient(int clientId) = 0;
};

#endif /* !INETWORK_PROVIDER_HPP_ */
