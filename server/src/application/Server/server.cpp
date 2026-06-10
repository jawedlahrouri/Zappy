/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** network.cpp
*/

#include "Server/Server.hpp"
#include <iostream>
#include "ServerException.hpp"
#include "Socket/Socket.hpp"
#include "Poll/Poll.hpp"
#include "Parsing/Parsing.hpp"
#include <chrono>
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Event/GUI/EventVisitor.hpp"
#include "Event/GUI/PdiEvent.hpp"
#include "Server/login.hpp"
#include "Client/Client.hpp"
#include "Client/AI.hpp"
#include "Client/GUI.hpp"

Server::Server(ILogger& logger) : m_logger(logger) {}

Server::~Server()
{
    try {
        cleanup();
    } catch (const std::exception &e) {
        m_logger.error(ServerException(std::string("Server::~Server: ") + e.what()).what());
    }
}

void Server::sendResponse(int clientId, const std::string& message)
{
    try {
        std::string msg_to_send = message;
        if (msg_to_send.empty() || msg_to_send.back() != '\n') {
            msg_to_send += "\n";
        }
        auto it = m_clients.find(clientId);
        if (it != m_clients.end()) {
            it->second->appendToWriteBuffer(msg_to_send);
            addPollEvent(clientId, SysPoll::EventOut);
        }
    } catch (const std::exception &e) {
        m_logger.error(std::string("[ERROR] sendResponse exception: ") + e.what());
    }
}

void Server::sendToGuis(const std::string& message)
{
    try {
        for (const auto &[fd, client] : m_clients) {
            if (client->getType() == ClientType::GUI) {
                sendResponse(fd, message);
            }
        }
    } catch (const std::exception &e) {
        m_logger.error(std::string("[ERROR] sendToGuis exception: ") + e.what());
    }
}

void Server::broadcastEvent(const std::string& message, int excludeClientId)
{
    try {
        for (const auto &[socket, client] : m_clients) {
            if (socket != excludeClientId) {
                sendResponse(socket, message);
            }
        }
    } catch (const std::exception &e) {
        m_logger.error(std::string("[ERROR] broadcastEvent exception: ") + e.what());
    }
}

void Server::sendToAi(const std::string& message)
{
    try {
        for (const auto &[fd, client] : m_clients) {
            if (client->getType() == ClientType::AI) {
                sendResponse(fd, message);
            }
        }
    } catch (const std::exception &e) {
        m_logger.error(std::string("[ERROR] sendToAi exception: ") + e.what());
    }
}

void Server::sendEvent(const IEventElement &event)
{
    try {
        EventVisitor visitor(*this);
        event.Accept(&visitor);
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::sendEvent: ") + e.what());
    }
}

void Server::detectClientType(int clientSocket)
{
    try {
        auto it = m_clients.find(clientSocket);
        std::string line;
        bool teamExists = false;
        
        if (it == m_clients.end() || it->second->getType() != ClientType::PENDING)
            return;

        std::shared_ptr<Client> &client = it->second;
        if (!client->extractLine(line))
            return;
        if (line == "GRAPHIC") {
            auto gui_client = std::make_shared<GUI>(std::move(client->getSocket()));
            m_clients[clientSocket] = gui_client;
            Login::login_gui(gui_client, *this);
            m_logger.info("Client " + std::to_string(clientSocket) + " authenticated as GUI");
        } else {
            for (const auto &team : m_teamNames) {
                if (line == team) {
                    teamExists = true;
                    break;
                }
            }
            if (teamExists) {
                int playerId = clientSocket;
                auto ai_client = std::make_shared<AI>(std::move(client->getSocket()), playerId);
                ai_client->setTeamName(line);
                m_clients[clientSocket] = ai_client;
                Login::login_ai(ai_client, line, *this);
                m_logger.info("Client " + std::to_string(clientSocket) + " authenticated as AI in team " + line);
            } else {
                sendResponse(clientSocket, "ko");
                removeClient(clientSocket);
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::detectClientType: ") + e.what());
    }
}

void Server::acceptClient()
{
    try {
        Socket client_socket = m_masterSocket.accept_connection();
        int fd = client_socket.getSocket();
        
        if (fd != -1) {
            client_socket.set_non_blocking(true);
            m_poll.add_fd(fd, SysPoll::EventIn);
            m_clients[fd] = std::make_shared<Client>(std::move(client_socket));
            m_logger.info("New client connected on socket " + std::to_string(fd));
            sendResponse(fd, "WELCOME");
        }
    } catch (const std::exception &e) {
        m_logger.error("Failed to accept client connection: " + std::string(e.what()));
    }
}

void Server::removeClient(int clientSocket)
{
    try {
        auto it = m_clients.find(clientSocket);
        if (it != m_clients.end()) {
            if (it->second->getType() == ClientType::AI) {
                auto ai = std::dynamic_pointer_cast<AI>(it->second);
                if (ai)
                    sendEvent(PdiEvent(ai->getId()));
            }
            if (!it->second->getWriteBuffer().empty()) {
                try {
                    it->second->getSocket().send_data(it->second->getWriteBuffer());
                } catch (...) {}
            }
            it->second->getSocket().close_socket();
            m_clients.erase(it);
        }
        m_poll.remove_fd(clientSocket);
        m_logger.info("Client " + std::to_string(clientSocket) + " disconnected");
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::removeClient: ") + e.what());
    }
}

bool Server::handleWritable(size_t index)
{
    try {
        int client_socket = m_poll.get_fd(index);
        auto it = m_clients.find(client_socket);
        if (it == m_clients.end()) {
            m_poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

        const auto &buffer = it->second->getWriteBuffer();
        if (buffer.empty()) {
            m_poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

        try {
            size_t sent = it->second->getSocket().send_data(buffer);
            if (sent > 0) {
                it->second->removeFromWriteBuffer(sent);
            }
            if (!it->second->hasWriteData()) {
                m_poll.remove_events(client_socket, SysPoll::EventOut);
            }
        } catch (const std::exception &e) {
            m_logger.error("handleWritable: " + std::string(e.what()));
            removeClient(client_socket);
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        m_logger.error("Server::handleWritable exception: " + std::string(e.what()));
        return false;
    }
}

bool Server::handleClientData(size_t pollIndex)
{
    try {
        int client_socket = m_poll.get_fd(pollIndex);
        auto it = m_clients.find(client_socket);
        if (it == m_clients.end()) {
            return false;
        }
        std::string data;
        try {
            data = it->second->getSocket().recv_data(1024);
        } catch (const std::exception &e) {
            m_logger.info("Client " + std::to_string(client_socket) + " disconnected: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (data.empty()) {
            return false;
        }

        try {
            it->second->appendToBuffer(data);
        } catch (const std::overflow_error &e) {
            m_logger.error("Client " + std::to_string(client_socket) + " buffer overflow: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (it->second->getType() == ClientType::PENDING) {
            detectClientType(client_socket);
        } else {
            std::string line;
            while (it->second->extractLine(line)) {
                if (it->second->getType() == ClientType::GUI) {
                    auto gui_client = std::dynamic_pointer_cast<GUI>(it->second);
                    Parsing::parse_gui_command(gui_client, line, *this);
                } else if (it->second->getType() == ClientType::AI) {
                    auto ai_client = std::dynamic_pointer_cast<AI>(it->second);
                    Parsing::parse_ai_command(ai_client, line, *this);
                }
            }
        }
        return false;
    } catch (const std::exception &e) {
        m_logger.error("Server::handleClientData exception: " + std::string(e.what()));
        return false;
    }
}

void Server::cleanup()
{
    try {
        for (auto &[fd, client] : m_clients) {
            client->getSocket().close_socket();
        }
        m_clients.clear();
        m_masterSocket.close_socket();
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::cleanup: ") + e.what());
    }
}

void Server::processPollEvent(size_t &i)
{
    try {
        if (m_poll.is_error(i)) {
            if (i > 0) {
                removeClient(m_poll.get_fd(i));
                --i;
            }
            return;
        }
        if (m_poll.is_readable(i)) {
            if (i == 0) {
                acceptClient();
            } else {
                if (handleClientData(i)) {
                    --i;
                    return;
                }
            }
        }
        if (i > 0 && i < m_poll.get_size() && m_poll.is_writable(i)) {
            if (handleWritable(i)) {
                --i;
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::processPollEvent: ") + e.what());
    }
}

void Server::runPollLoop()
{
    try {
        m_logger.info("Waiting for connections...");
        std::chrono::microseconds tick_duration = std::chrono::microseconds(1000000 / m_freq);
        auto next_tick_time = std::chrono::steady_clock::now();

        while (true) {
            auto now = std::chrono::steady_clock::now();
            int timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(next_tick_time - now).count();
            if (timeout_ms < 0) {
                timeout_ms = 0;
            }

            int poll_count = m_poll.run_poll(timeout_ms);
            if (poll_count == -1) {
                if (SysPoll::isInterrupted()) {
                    continue;
                }
                throw ServerException(std::string("poll failed: ") + SysPoll::getLastError());
            }

            for (size_t i = 0; i < m_poll.get_size(); ++i) {
                processPollEvent(i);
            }

            now = std::chrono::steady_clock::now();
            if (now >= next_tick_time) {
                if (m_gameEngine) {
                    m_gameEngine->update(*this);
                    std::vector<int> clientsToRemove;
                    for (auto &[fd, client] : m_clients) {
                        client->update(client, *this, *m_gameEngine, clientsToRemove);
                    }
                    for (int fd : clientsToRemove) {
                        removeClient(fd);
                    }
                }
                next_tick_time += tick_duration;
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("runPollLoop: ") + e.what());
    }
}

void Server::openServer(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq, int width, int height)
{
    try {
    m_port = port;
    m_clientsNb = clientsNb;
    m_teamNames = teamNames;
    m_freq = freq;
    try {
        m_masterSocket.create_socket(port, clientsNb);
        m_masterSocket.bind_socket();
        m_masterSocket.receive_data();

        m_logger.info("Server opened on port " + std::to_string(m_port));
        m_poll.add_fd(m_masterSocket.getSocket(), SysPoll::EventIn);

        m_gameEngine = std::make_unique<GameEngine>(width, height, m_logger);

        runPollLoop();
    } catch (const std::exception &e) {
        throw ServerException(std::string("openServer: ") + e.what());
    }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::openServer: ") + e.what());
    }
}

const std::unordered_map<int, std::shared_ptr<Client>>& Server::getClients() const
{
    return m_clients;
}

void Server::addPollEvent(int fd, short events)
{
    m_poll.add_events(fd, events);
}

int Server::getClientsNb() const
{
    return m_clientsNb;
}

int Server::getFreq() const
{
    return m_freq;
}

const std::vector<std::string>& Server::getTeamNames() const
{
    return m_teamNames;
}

const std::unique_ptr<GameEngine>& Server::getGameEngine() const
{
    return m_gameEngine;
}

ILogger& Server::getLogger() const
{
    return m_logger;
}