/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ClientManager.cpp
*/

#include "application/Server/ClientManager.hpp"
#include "INetworkProvider.hpp"
#include "shared/ILogger.hpp"
#include "application/Event/GUI/PdiEvent.hpp"
#include "AI.hpp"
#include "domain/GameEngine/GameEngine.hpp"
#include "ServerException.hpp"

ClientManager::ClientManager(INetworkProvider& network, ILogger& logger)
    : _network(network), _logger(logger)
{
}

void ClientManager::addClient(int fd, std::shared_ptr<Client> client)
{
    _clients[fd] = std::move(client);
}

/**
 * @brief Removes a client, notifying GUIs and flushing its buffer first.
 * @details For an AI it emits a @c pdi (player death/leave) event and clears the
 *          player from its map tile; then it best-effort flushes any pending
 *          output, closes the socket and erases the entry.
 */
void ClientManager::removeClient(int fd)
{
    auto it = _clients.find(fd);
    if (it != _clients.end()) {
        if (it->second->getType() == ClientType::AI) {
            auto ai = std::dynamic_pointer_cast<AI>(it->second);
            if (ai) {
                _network.sendEvent(PdiEvent(ai->getId()));
                if (_network.getGameEngine()) {
                    _network.getGameEngine()->getMap().removeAiFromTile(ai->getId(), ai->getX(), ai->getY());
                }
            }
        }
        if (!it->second->getWriteBuffer().empty()) {
            try {
                it->second->getSocket().sendData(it->second->getWriteBuffer());
            } catch (const std::exception& e) {
                _logger.warn("Failed to flush write buffer for client " + std::to_string(fd) + " before disconnect: " + e.what());
            }
        }
        it->second->getSocket().closeSocket();
        _clients.erase(it);
    }
}

std::shared_ptr<Client> ClientManager::getClient(int fd)
{
    auto it = _clients.find(fd);
    if (it != _clients.end()) {
        return it->second;
    }
    return nullptr;
}

const std::unordered_map<int, std::shared_ptr<Client>>& ClientManager::getClients() const
{
    return _clients;
}

/**
 * @brief Queues a message and arms POLLOUT so the loop will flush it.
 * @details Ensures a trailing newline (the line terminator of every Zappy
 *          message), appends to the client's write buffer, then requests the
 *          @c Output poll event — the trigger for @ref Server::handleWritable.
 */
void ClientManager::sendResponse(int clientId, const std::string& message)
{
    try {
        std::string msg_to_send = message;
        if (msg_to_send.empty() || msg_to_send.back() != '\n') {
            msg_to_send += "\n";
        }
        auto it = _clients.find(clientId);
        if (it != _clients.end()) {
            it->second->appendToWriteBuffer(msg_to_send);
            _network.addPollEvent(clientId, NetworkEvent::Output);
        }
    } catch (const std::exception &e) {
        _logger.error(std::string("[ERROR] sendResponse exception: ") + e.what());
    }
}

void ClientManager::sendToGuis(const std::string& message)
{
    try {
        for (const auto &[fd, client] : _clients) {
            if (client->getType() == ClientType::GUI) {
                sendResponse(fd, message);
            }
        }
    } catch (const std::exception &e) {
        _logger.error(std::string("[ERROR] sendToGuis exception: ") + e.what());
    }
}

void ClientManager::sendToAi(const std::string& message)
{
    try {
        for (const auto &[fd, client] : _clients) {
            if (client->getType() == ClientType::AI) {
                sendResponse(fd, message);
            }
        }
    } catch (const std::exception &e) {
        _logger.error(std::string("[ERROR] sendToAi exception: ") + e.what());
    }
}

void ClientManager::broadcastEvent(const std::string& message, int excludeClientId)
{
    try {
        for (const auto &[socket, client] : _clients) {
            if (socket != excludeClientId) {
                sendResponse(socket, message);
            }
        }
    } catch (const std::exception &e) {
        _logger.error(std::string("[ERROR] broadcastEvent exception: ") + e.what());
    }
}

void ClientManager::disconnectAll()
{
    for (auto &[fd, client] : _clients) {
        client->getSocket().closeSocket();
    }
    _clients.clear();
}
