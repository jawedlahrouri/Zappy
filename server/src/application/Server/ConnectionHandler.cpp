/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectionHandler.cpp
*/

#include "application/Server/ConnectionHandler.hpp"
#include "application/Server/ClientManager.hpp"
#include "INetworkProvider.hpp"
#include "shared/ILogger.hpp"
#include "application/Server/login.hpp"
#include "ISocket.hpp"
#include "IPoll.hpp"
#include "AI.hpp"
#include "GUI.hpp"
#include "ServerException.hpp"

void ConnectionHandler::acceptClient(ISocket& masterSocket, IPoll& poll, ClientManager& clientManager, ILogger& logger)
{
    try {
        std::unique_ptr<ISocket> client_socket = masterSocket.acceptConnection();
        if (!client_socket) {
            return;
        }
        int fd = client_socket->getSocket();
        
        if (fd != -1) {
            client_socket->setNonBlocking(true);
            poll.addFd(fd, NetworkEvent::Input);
            clientManager.addClient(fd, std::make_shared<Client>(std::move(client_socket), fd));
            logger.info("New client connected on socket " + std::to_string(fd));
            clientManager.sendResponse(fd, "WELCOME");
        }
    } catch (const std::exception &e) {
        logger.error("Failed to accept client connection: " + std::string(e.what()));
    }
}

void ConnectionHandler::detectClientType(int clientSocket, ClientManager& clientManager, INetworkProvider& network, ILogger& logger, const std::vector<std::string>& teamNames)
{
    try {
        std::shared_ptr<Client> client = clientManager.getClient(clientSocket);
        if (!client || client->getType() != ClientType::PENDING)
            return;

        std::string line;
        bool teamExists = false;
        
        if (!client->extractLine(line))
            return;
        if (line == "GRAPHIC") {
            auto gui_client = std::make_shared<GUI>(client->releaseSocket(), clientSocket);
            clientManager.addClient(clientSocket, gui_client);
            Login::login_gui(gui_client, network);
            logger.info("Client " + std::to_string(clientSocket) + " authenticated as GUI");
        } else {
            for (const auto &team : teamNames) {
                if (line == team) {
                    teamExists = true;
                    break;
                }
            }
            if (teamExists) {
                int playerId = clientSocket;
                auto ai_client = std::make_shared<AI>(client->releaseSocket(), playerId);
                ai_client->setTeamName(line);
                clientManager.addClient(clientSocket, ai_client);
                if (Login::login_ai(ai_client, line, network)) {
                    logger.info("Client " + std::to_string(clientSocket) + " authenticated as AI in team " + line);
                } else {
                    logger.warn("Client " + std::to_string(clientSocket) + " could not join team " + line + ": no slots available");
                    network.removeClient(clientSocket);
                }
            } else {
                clientManager.sendResponse(clientSocket, "ko");
                network.removeClient(clientSocket);
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("ConnectionHandler::detectClientType: ") + e.what());
    }
}
