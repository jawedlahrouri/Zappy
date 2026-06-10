/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.hpp
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <map>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Socket/Socket.hpp"
#include "shared/ILogger.hpp"
#include "Poll/Poll.hpp"

#include "Network/INetworkProvider.hpp"
#include "Client/Client.hpp"

class AI;
class GUI;
class GameEngine;

class Server : public INetworkProvider {
    public:
        explicit Server(ILogger& logger);
        ~Server() override;

        void openServer(int port, int clientsNb, const std::vector<std::string>& teamNames, int freq, int width, int height);
        void removeClient(int clientSocket);
        const std::unordered_map<int, std::shared_ptr<Client>>& getClients() const;
        void addPollEvent(int fd, short events);
        int getClientsNb() const;
        int getFreq() const;
        const std::vector<std::string>& getTeamNames() const;
        const std::unique_ptr<GameEngine>& getGameEngine() const;
        void sendEvent(const class IEventElement& event);
        ILogger& getLogger() const;

        // INetworkProvider Implementation
        void sendResponse(int clientId, const std::string& message) override;
        void sendToGuis(const std::string& message) override;
        void broadcastEvent(const std::string& message, int excludeClientId = -1) override;
        void sendToAi(const std::string& message);

    private:
        ILogger& m_logger;
        void detectClientType(int clientSocket);
        void runPollLoop();
        void processPollEvent(size_t& i);
        void acceptClient();
        bool handleClientData(size_t pollIndex);
        bool handleWritable(size_t index);
        void cleanup();

        Socket m_masterSocket;
        Poll m_poll;
        std::unordered_map<int, std::shared_ptr<Client>> m_clients;
        std::unique_ptr<GameEngine> m_gameEngine;

        int m_port = 0;
        int m_clientsNb = 0;
        std::vector<std::string> m_teamNames;
        int m_freq = 0;
};

#endif /* !SERVER_HPP_ */