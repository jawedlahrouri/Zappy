/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <vector>
#include <memory>
#include "Network/ISocket.hpp"
#include "shared/CircularBuffer.hpp"
#include "Network/INetworkProvider.hpp"

class GameEngine;

enum class ClientType {
    PENDING,
    AI,
    GUI
};

class Client {
    public:
        Client(std::unique_ptr<ISocket> clientSocket, int id);
        virtual ~Client() = default;

        ISocket& getSocket() { return *m_socket; }
        const ISocket& getSocket() const { return *m_socket; }
        std::unique_ptr<ISocket> releaseSocket() { return std::move(m_socket); }
        
        int getId() const { return m_id; }
        
        ClientType getType() const { return m_type; }
        void setType(ClientType newType) { m_type = newType; }

        void appendToBuffer(const std::string& data);
        bool extractLine(std::string& line);
        void appendToWriteBuffer(const std::string& data);
        std::string getWriteBuffer() const;
        bool hasWriteData() const;
        void clearWriteBuffer();
        void removeFromWriteBuffer(size_t len);

        virtual void update(std::shared_ptr<Client>, INetworkProvider&, GameEngine&, std::vector<int>&) {}

    protected:
        std::unique_ptr<ISocket> m_socket;
        int m_id;
        ClientType m_type;

    private:
        CircularBuffer m_readBuffer;
        CircularBuffer m_writeBuffer;
};

#endif /* !CLIENT_HPP_ */
