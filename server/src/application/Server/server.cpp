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
#include "application/Event/GUI/SegEvent.hpp"
#include "AI.hpp"
#include "GUI.hpp"
#include <chrono>
#include "application/Server/Handlers/PendingClientHandler.hpp"
#include "presentation/Handlers/AiClientHandler.hpp"
#include "presentation/Handlers/GuiClientHandler.hpp"

Server::Server(ILogger& logger, std::unique_ptr<ISocket> masterSocket, std::unique_ptr<IPoll> poll)
    : _logger(logger), _masterSocket(std::move(masterSocket)), _poll(std::move(poll)), _clientManager(*this, logger)
{
    _dataHandlers[ClientType::PENDING] = std::make_unique<PendingClientHandler>();
    _dataHandlers[ClientType::AI] = std::make_unique<AiClientHandler>();
    _dataHandlers[ClientType::GUI] = std::make_unique<GuiClientHandler>();
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

ClientManager& Server::getClientManager()
{
    return _clientManager;
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

        auto handler = _dataHandlers.find(client->getType());
        if (handler != _dataHandlers.end()) {
            handler->second->handleData(client, *this);
        } else {
            _logger.warn("No data handler found for client type");
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

void Server::processPollEvent(size_t i)
{
    try {
        if (_poll->isError(i)) {
            if (i > 0) {
                removeClient(_poll->getFd(i));
            }
            return;
        }
        if (_poll->isReadable(i)) {
            if (i == 0) {
                ConnectionHandler::acceptClient(*_masterSocket, *_poll, _clientManager, _logger);
            } else {
                if (handleClientData(i)) {
                    return;
                }
            }
        }
        if (i > 0 && i < _poll->getSize() && _poll->isWritable(i)) {
            handleWritable(i);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::processPollEvent: ") + e.what());
    }
}

int Server::calculateTimeout(const std::chrono::steady_clock::time_point& next_tick_time) const
{
    if (_clientManager.getClients().empty()) {
        return -1;
    }
    auto now = std::chrono::steady_clock::now();
    auto diff = next_tick_time - now;
    if (diff > std::chrono::steady_clock::duration::zero()) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            diff + std::chrono::microseconds(999)).count();
    }
    return 0;
}

void Server::handlePollEvents(int timeout_ms)
{
    int poll_count = _poll->runPoll(timeout_ms);
    if (poll_count == -1) {
        throw ServerException(std::string("poll failed"));
    }

    size_t i = _poll->getSize();
    while (i > 0) {
        --i;
        if (i < _poll->getSize()) {
            processPollEvent(i);
        }
    }
}

void Server::synchronizeTickTime(bool& was_empty, std::chrono::steady_clock::time_point& next_tick_time, const std::chrono::microseconds& tick_duration) const
{
    auto now = std::chrono::steady_clock::now();
    bool is_empty = _clientManager.getClients().empty();

    if (is_empty) {
        was_empty = true;
        next_tick_time = now + tick_duration;
    } else if (was_empty) {
        was_empty = false;
        next_tick_time = now + tick_duration;
    }
}

void Server::runGameTick(std::chrono::steady_clock::time_point& next_tick_time, const std::chrono::microseconds& tick_duration)
{
    auto now = std::chrono::steady_clock::now();
    while (now >= next_tick_time) {
        if (_gameEngine && !_clientManager.getClients().empty()) {
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
        now = std::chrono::steady_clock::now();
    }
}

void Server::runPollLoop()
{
    try {
        _logger.info("Waiting for connections...");
        auto next_tick_time = std::chrono::steady_clock::now() + std::chrono::microseconds(1000000 / _freq);
        bool was_empty = true;

        while (true) {
            std::chrono::microseconds tick_duration = std::chrono::microseconds(1000000 / _freq);
            int timeout_ms = calculateTimeout(next_tick_time);
            handlePollEvents(timeout_ms);
            synchronizeTickTime(was_empty, next_tick_time, tick_duration);
            runGameTick(next_tick_time, tick_duration);
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
            _gameEngine->generateInitialEggs(teamNames, clientsNb);

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

void Server::setClientsNb(int nb)
{
    _clientsNb = nb;
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

void Server::executeCommand(std::shared_ptr<AI> ai, const std::string& command)
{
    try {
        AiCommandParser parser;
        parser.executeCommand(ai, command, *this);
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::executeCommand: ") + e.what());
    }
}

void Server::notifyVictory(const std::string& teamName)
{
    try {
        _logger.info("[VICTORY] Team '" + teamName + "' wins the game (6 players reached level 8).");
        sendEvent(SegEvent(teamName));
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::notifyVictory: ") + e.what());
    }
}

int Server::getCommandDuration(const std::string& command) const
{
    try {
        std::string action = command.substr(0, command.find(' '));
        auto it = ai_cmds.find(action);
        if (it != ai_cmds.end()) {
            return it->second.duration;
        }
        return 0;
    } catch (const std::exception &e) {
        throw ServerException(std::string("Server::getCommandDuration: ") + e.what());
    }
}
void Server::setFreq(int freq)
{
    _freq = freq;
}
