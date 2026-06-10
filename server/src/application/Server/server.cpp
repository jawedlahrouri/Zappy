/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.cpp
*/

#include "Server/Server.hpp"
#include <iostream>
#include "ServerException.hpp"
#include "Parsing/Parsing.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Event/GUI/EventVisitor.hpp"
#include "Event/GUI/IEventElement.hpp"
#include "Client/AI.hpp"
#include "Client/GUI.hpp"
#include "Poll/Sys_Poll.hpp"
#include <chrono>

Server::Server(ILogger& logger) : m_logger(logger), m_clientManager(*this, logger) {}

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
    m_clientManager.sendResponse(clientId, message);
}

void Server::sendToGuis(const std::string& message)
{
    m_clientManager.sendToGuis(message);
}

void Server::broadcastEvent(const std::string& message, int excludeClientId)
{
    m_clientManager.broadcastEvent(message, excludeClientId);
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

const std::unordered_map<int, std::shared_ptr<Client>>& Server::getClients() const
{
    return m_clientManager.getClients();
}

void Server::addPollEvent(int fd, short events)
{
    m_poll.add_events(fd, events);
}

void Server::removeClient(int clientSocket)
{
    try {
        m_clientManager.removeClient(clientSocket);
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
        auto client = m_clientManager.getClient(client_socket);
        if (!client) {
            m_poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

        const auto &buffer = client->getWriteBuffer();
        if (buffer.empty()) {
            m_poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

        try {
            size_t sent = client->getSocket().send_data(buffer);
            if (sent > 0) {
                client->removeFromWriteBuffer(sent);
            }
            if (!client->hasWriteData()) {
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
        auto client = m_clientManager.getClient(client_socket);
        if (!client) {
            return false;
        }
        std::string data;
        try {
            data = client->getSocket().recv_data(1024);
        } catch (const std::exception &e) {
            m_logger.info("Client " + std::to_string(client_socket) + " disconnected: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (data.empty()) {
            return false;
        }

        try {
            client->appendToBuffer(data);
        } catch (const std::overflow_error &e) {
            m_logger.error("Client " + std::to_string(client_socket) + " buffer overflow: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (client->getType() == ClientType::PENDING) {
            ConnectionHandler::detectClientType(client_socket, m_clientManager, *this, m_logger, m_teamNames);
        } else {
            std::string line;
            while (client->extractLine(line)) {
                if (client->getType() == ClientType::GUI) {
                    auto gui_client = std::dynamic_pointer_cast<GUI>(client);
                    Parsing::parse_gui_command(gui_client, line, *this);
                } else if (client->getType() == ClientType::AI) {
                    auto ai_client = std::dynamic_pointer_cast<AI>(client);
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
        m_clientManager.disconnectAll();
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
                ConnectionHandler::acceptClient(m_masterSocket, m_poll, m_clientManager, m_logger);
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
                    for (auto &[fd, client] : m_clientManager.getClients()) {
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
