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

        ISocket& getSocket() { return *_socket; }
        const ISocket& getSocket() const { return *_socket; }
        std::unique_ptr<ISocket> releaseSocket() { return std::move(_socket); }
        
        int getId() const { return _id; }
        
        ClientType getType() const { return _type; }
        void setType(ClientType newType) { _type = newType; }

        void appendToBuffer(const std::string& data);
        bool extractLine(std::string& line);
        void appendToWriteBuffer(const std::string& data);
        std::string getWriteBuffer() const;
        bool hasWriteData() const;
        void clearWriteBuffer();
        void removeFromWriteBuffer(size_t len);

        virtual void update(std::shared_ptr<Client>, INetworkProvider&, GameEngine&, std::vector<int>&) {}

    protected:
        std::unique_ptr<ISocket> _socket;
        int _id;
        ClientType _type;

    private:
        CircularBuffer _readBuffer;
        CircularBuffer _writeBuffer;
};

#endif /* !CLIENT_HPP_ */
