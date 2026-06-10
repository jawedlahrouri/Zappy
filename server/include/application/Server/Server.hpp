/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.hpp
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <vector>
#include <memory>
#include "Socket/Socket.hpp"
#include "shared/ILogger.hpp"
#include "Poll/Poll.hpp"

#include "Network/INetworkProvider.hpp"
#include "Server/ClientManager.hpp"
#include "Server/ConnectionHandler.hpp"

class GameEngine;

class Server : public INetworkProvider {
    public:
        explicit Server(ILogger& logger);
        ~Server() override;

        void openServer(int port, int clientsNb, const std::vector<std::string>& teamNames, int freq, int width, int height);

        int getClientsNb() const;
        int getFreq() const;
        const std::vector<std::string>& getTeamNames() const;
        const std::unique_ptr<GameEngine>& getGameEngine() const;
        ILogger& getLogger() const;

        // INetworkProvider Implementation
        void sendResponse(int clientId, const std::string& message) override;
        void sendToGuis(const std::string& message) override;
        void broadcastEvent(const std::string& message, int excludeClientId = -1) override;
        void sendEvent(const class IEventElement& event) override;
        const std::unordered_map<int, std::shared_ptr<class Client>>& getClients() const override;
        void addPollEvent(int fd, short events) override;

    private:
        void runPollLoop();
        void processPollEvent(size_t& i);
        bool handleClientData(size_t pollIndex);
        bool handleWritable(size_t index);
        void cleanup();
        void removeClient(int clientSocket);

        ILogger& m_logger;
        Socket m_masterSocket;
        Poll m_poll;
        ClientManager m_clientManager;
        std::unique_ptr<GameEngine> m_gameEngine;

        int m_port = 0;
        int m_clientsNb = 0;
        std::vector<std::string> m_teamNames;
        int m_freq = 0;
};

#endif /* !SERVER_HPP_ */
