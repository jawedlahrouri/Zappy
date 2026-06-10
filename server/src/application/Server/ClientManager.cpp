/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ClientManager.cpp
*/

#include "Server/ClientManager.hpp"
#include "Network/INetworkProvider.hpp"
#include "shared/ILogger.hpp"
#include "Event/GUI/PdiEvent.hpp"
#include "Client/AI.hpp"
#include "ServerException.hpp"

ClientManager::ClientManager(INetworkProvider& network, ILogger& logger)
    : m_network(network), m_logger(logger)
{
}

void ClientManager::addClient(int fd, std::shared_ptr<Client> client)
{
    m_clients[fd] = std::move(client);
}

void ClientManager::removeClient(int fd)
{
    auto it = m_clients.find(fd);
    if (it != m_clients.end()) {
        if (it->second->getType() == ClientType::AI) {
            auto ai = std::dynamic_pointer_cast<AI>(it->second);
            if (ai)
                m_network.sendEvent(PdiEvent(ai->getId()));
        }
        if (!it->second->getWriteBuffer().empty()) {
            try {
                it->second->getSocket().sendData(it->second->getWriteBuffer());
            } catch (const std::exception& e) {
                m_logger.warn("Failed to flush write buffer for client " + std::to_string(fd) + " before disconnect: " + e.what());
            }
        }
        it->second->getSocket().closeSocket();
        m_clients.erase(it);
    }
}

std::shared_ptr<Client> ClientManager::getClient(int fd)
{
    auto it = m_clients.find(fd);
    if (it != m_clients.end()) {
        return it->second;
    }
    return nullptr;
}

const std::unordered_map<int, std::shared_ptr<Client>>& ClientManager::getClients() const
{
    return m_clients;
}

void ClientManager::sendResponse(int clientId, const std::string& message)
{
    try {
        std::string msg_to_send = message;
        if (msg_to_send.empty() || msg_to_send.back() != '\n') {
            msg_to_send += "\n";
        }
        auto it = m_clients.find(clientId);
        if (it != m_clients.end()) {
            it->second->appendToWriteBuffer(msg_to_send);
            m_network.addPollEvent(clientId, NetworkEvent::Output);
        }
    } catch (const std::exception &e) {
        m_logger.error(std::string("[ERROR] sendResponse exception: ") + e.what());
    }
}

void ClientManager::sendToGuis(const std::string& message)
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

void ClientManager::sendToAi(const std::string& message)
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

void ClientManager::broadcastEvent(const std::string& message, int excludeClientId)
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

void ClientManager::disconnectAll()
{
    for (auto &[fd, client] : m_clients) {
        client->getSocket().closeSocket();
    }
    m_clients.clear();
}
