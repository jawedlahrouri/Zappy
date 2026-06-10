/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectionHandler.cpp
*/

#include "Server/ConnectionHandler.hpp"
#include "Server/ClientManager.hpp"
#include "Network/INetworkProvider.hpp"
#include "shared/ILogger.hpp"
#include "Server/login.hpp"
#include "Socket/Socket.hpp"
#include "Poll/Poll.hpp"
#include "Poll/Sys_Poll.hpp"
#include "Client/AI.hpp"
#include "Client/GUI.hpp"
#include "ServerException.hpp"

void ConnectionHandler::acceptClient(Socket& masterSocket, Poll& poll, ClientManager& clientManager, ILogger& logger)
{
    try {
        Socket client_socket = masterSocket.accept_connection();
        int fd = client_socket.getSocket();
        
        if (fd != -1) {
            client_socket.set_non_blocking(true);
            poll.add_fd(fd, SysPoll::EventIn);
            clientManager.addClient(fd, std::make_shared<Client>(std::move(client_socket)));
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
            auto gui_client = std::make_shared<GUI>(std::move(client->getSocket()));
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
                auto ai_client = std::make_shared<AI>(std::move(client->getSocket()), playerId);
                ai_client->setTeamName(line);
                clientManager.addClient(clientSocket, ai_client);
                Login::login_ai(ai_client, line, network);
                logger.info("Client " + std::to_string(clientSocket) + " authenticated as AI in team " + line);
            } else {
                clientManager.sendResponse(clientSocket, "ko");
                clientManager.removeClient(clientSocket);
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("ConnectionHandler::detectClientType: ") + e.what());
    }
}
