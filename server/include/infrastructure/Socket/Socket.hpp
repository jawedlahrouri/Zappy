/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.hpp
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "ISocket.hpp"
#include "INetworkLoader.hpp"

class Socket : public ISocket {
    public:
        explicit Socket(const NetworkBackend::Symbols &symbols);
        explicit Socket(int fd, const NetworkBackend::Symbols &symbols);
        ~Socket() override = default;

        void createSocket(int port, int backlog) override;
        void closeSocket() override;
        void bindSocket() override;
        std::unique_ptr<ISocket> acceptConnection() override;
        void receiveData() override;
        size_t sendData(const std::string &data) override;
        std::string recvData(size_t maxSize) override;
        void setNonBlocking(bool nonBlocking) override;

        int getSocket() const override;
        int getClientSocket() const override;

    private:
        const NetworkBackend::Symbols &_symbols;
        int _socket;
        int _port;
        int _backlog;
        int _clientSocket;
        std::string _data;
};

#endif /* !SOCKET_HPP_ */