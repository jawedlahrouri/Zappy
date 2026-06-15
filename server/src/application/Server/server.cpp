/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.cpp
*/

#include "application/Server/Server.hpp"
#include <iostream>
#include "ServerException.hpp"
#include "Parsing.hpp"
#include "AiCommandParser.hpp"
#include "ai_execute_cmds.hpp"
#include "GameEngine.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "application/Event/GUI/IEventElement.hpp"
#include "AI.hpp"
#include "GUI.hpp"
#include <chrono>

Server::Server(ILogger& logger, std::unique_ptr<ISocket> masterSocket, std::unique_ptr<IPoll> poll)
    : _logger(logger), _masterSocket(std::move(masterSocket)), _poll(std::move(poll)), _clientManager(*this, logger)
{
}

Server::~Server()
{
    try {
        cleanup();
    } catch (const std::exception &e) {
        _logger.error(ServerException(std::string("Server::~Server: ") + e.what()).what());
    }
}

void Server::sendResponse(int clientId, const std::string& message)
{
    _clientManager.sendResponse(clientId, message);
}

void Server::sendToGuis(const std::string& message)
{
    _clientManager.sendToGuis(message);
}

void Server::broadcastEvent(const std::string& message, int excludeClientId)
{
    _clientManager.broadcastEvent(message, excludeClientId);
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
    return _clientManager.getClients();
}

void Server::addPollEvent(int fd, NetworkEvent event)
{
    _poll->addEvents(fd, event);
}

void Server::removeClient(int clientSocket)
{
    try {
        _clientManager.removeClient(clientSocket);
        _poll->removeFd(clientSocket);
        _logger.info("Client " + std::to_string(clientSocket) + " disconnected");
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::removeClient: ") + e.what());
    }
}

bool Server::handleWritable(size_t index)
{
    try {
        int client_socket = _poll->getFd(index);
        auto client = _clientManager.getClient(client_socket);
        if (!client) {
            _poll->removeEvents(client_socket, NetworkEvent::Output);
            return false;
        }

        const auto &buffer = client->getWriteBuffer();
        if (buffer.empty()) {
            _poll->removeEvents(client_socket, NetworkEvent::Output);
            return false;
        }

        try {
            size_t sent = client->getSocket().sendData(buffer);
            if (sent > 0) {
                client->removeFromWriteBuffer(sent);
            }
            if (!client->hasWriteData()) {
                _poll->removeEvents(client_socket, NetworkEvent::Output);
            }
        } catch (const std::exception &e) {
            _logger.error("handleWritable: " + std::string(e.what()));
            removeClient(client_socket);
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        _logger.error("Server::handleWritable exception: " + std::string(e.what()));
        return false;
    }
}

bool Server::handleClientData(size_t pollIndex)
{
    try {
        int client_socket = _poll->getFd(pollIndex);
        auto client = _clientManager.getClient(client_socket);
        if (!client) {
            return false;
        }
        std::string data;
        try {
            data = client->getSocket().recvData(1024);
        } catch (const std::exception &e) {
            _logger.info("Client " + std::to_string(client_socket) + " disconnected: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (data.empty()) {
            return false;
        }

        try {
            client->appendToBuffer(data);
        } catch (const std::overflow_error &e) {
            _logger.error("Client " + std::to_string(client_socket) + " buffer overflow: " + e.what());
            removeClient(client_socket);
            return true;
        }

        if (client->getType() == ClientType::PENDING) {
            ConnectionHandler::detectClientType(client_socket, _clientManager, *this, _logger, _teamNames);
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
        _logger.error("Server::handleClientData exception: " + std::string(e.what()));
        return false;
    }
}

void Server::cleanup()
{
    try {
        _clientManager.disconnectAll();
        _masterSocket->closeSocket();
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::cleanup: ") + e.what());
    }
}

void Server::processPollEvent(size_t &i)
{
    try {
        if (_poll->isError(i)) {
            if (i > 0) {
                removeClient(_poll->getFd(i));
                --i;
            }
            return;
        }
        if (_poll->isReadable(i)) {
            if (i == 0) {
                ConnectionHandler::acceptClient(*_masterSocket, *_poll, _clientManager, _logger);
            } else {
                if (handleClientData(i)) {
                    --i;
                    return;
                }
            }
        }
        if (i > 0 && i < _poll->getSize() && _poll->isWritable(i)) {
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
        _logger.info("Waiting for connections...");
        std::chrono::microseconds tick_duration = std::chrono::microseconds(1000000 / _freq);
        auto next_tick_time = std::chrono::steady_clock::now();

        while (true) {
            auto now = std::chrono::steady_clock::now();
            int timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(next_tick_time - now).count();
            if (timeout_ms < 0) {
                timeout_ms = 0;
            }

            int poll_count = _poll->runPoll(timeout_ms);
            if (poll_count == -1) {
                throw ServerException(std::string("poll failed"));
            }

            for (size_t i = 0; i < _poll->getSize(); ++i) {
                processPollEvent(i);
            }

            now = std::chrono::steady_clock::now();
            if (now >= next_tick_time) {
                if (_gameEngine) {
                    _gameEngine->update(*this);
                    std::vector<int> clientsToRemove;
                    for (auto &[fd, client] : _clientManager.getClients()) {
                        client->update(client, *this, *_gameEngine, clientsToRemove);
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
        _port = port;
        _clientsNb = clientsNb;
        _teamNames = teamNames;
        _freq = freq;
        try {
            _masterSocket->createSocket(port, clientsNb);
            _masterSocket->bindSocket();
            _masterSocket->receiveData();

            _logger.info("Server opened on port " + std::to_string(_port));
            _poll->addFd(_masterSocket->getSocket(), NetworkEvent::Input);

            _gameEngine = std::make_unique<GameEngine>(width, height, _logger);

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
    return _clientsNb;
}

int Server::getFreq() const
{
    return _freq;
}

const std::vector<std::string>& Server::getTeamNames() const
{
    return _teamNames;
}

const std::unique_ptr<GameEngine>& Server::getGameEngine() const
{
    return _gameEngine;
}

ILogger& Server::getLogger() const
{
    return _logger;
}
