/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ISocket.hpp
*/

#ifndef ISOCKET_HPP_
#define ISOCKET_HPP_

#include <string>
#include <memory>

class ISocket {
    public:
        virtual ~ISocket() = default;

        virtual void createSocket(int port, int backlog) = 0;
        virtual void bindSocket() = 0;
        virtual void receiveData() = 0;
        virtual std::unique_ptr<ISocket> acceptConnection() = 0;
        virtual size_t sendData(const std::string &data) = 0;
        virtual std::string recvData(size_t maxSize) = 0;
        virtual void setNonBlocking(bool nonBlocking) = 0;
        virtual void closeSocket() = 0;
        virtual int getSocket() const = 0;
        virtual int getClientSocket() const = 0;
};

#endif /* !ISOCKET_HPP_ */
