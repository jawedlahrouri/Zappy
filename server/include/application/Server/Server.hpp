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
#include "ISocket.hpp"
#include "shared/ILogger.hpp"
#include "IPoll.hpp"

#include "INetworkProvider.hpp"
#include "application/Server/ClientManager.hpp"
#include "application/Server/ConnectionHandler.hpp"

class GameEngine;

class Server : public INetworkProvider {
    public:
        Server(ILogger& logger, std::unique_ptr<ISocket> masterSocket, std::unique_ptr<IPoll> poll);
        ~Server() override;

        void openServer(int port, int clientsNb, const std::vector<std::string>& teamNames, int freq, int width, int height);

        int getClientsNb() const override;
        void setClientsNb(int nb) override;
        int getFreq() const override;
        const std::vector<std::string>& getTeamNames() const override;
        const std::unique_ptr<GameEngine>& getGameEngine() const override;
        ILogger& getLogger() const override;

        // INetworkProvider Implementation
        void sendResponse(int clientId, const std::string& message) override;
        void sendToGuis(const std::string& message) override;
        void broadcastEvent(const std::string& message, int excludeClientId = -1) override;
        void sendEvent(const class IEventElement& event) override;
        const std::unordered_map<int, std::shared_ptr<class Client>>& getClients() const override;
        void addPollEvent(int fd, NetworkEvent event) override;

    private:
        void runPollLoop();
        void processPollEvent(size_t& i);
        bool handleClientData(size_t pollIndex);
        bool handleWritable(size_t index);
        void cleanup();
        void removeClient(int clientSocket);

        ILogger& _logger;
        std::unique_ptr<ISocket> _masterSocket;
        std::unique_ptr<IPoll> _poll;
        ClientManager _clientManager;
        std::unique_ptr<GameEngine> _gameEngine;

        int _port = 0;
        int _clientsNb = 0;
        std::vector<std::string> _teamNames;
        int _freq = 0;
};

#endif /* !SERVER_HPP_ */
