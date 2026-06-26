/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.hpp
*/

/**
 * @file Socket.hpp
 * @brief Concrete @ref ISocket backed by the dynamically loaded backend.
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "ISocket.hpp"
#include "INetworkLoader.hpp"

/**
 * @class Socket
 * @brief @ref ISocket implementation forwarding to a network backend.
 *
 * @par Design Pattern: Adapter
 * Adapts the C socket functions exposed by a loaded backend
 * (@ref NetworkBackend::Symbols) to the @ref ISocket interface. The listening
 * server socket and each accepted client socket are both @c Socket instances.
 */
class Socket : public ISocket {
    public:
        /**
         * @brief Builds an unbound socket over a resolved backend.
         * @param symbols Backend function table supplying the socket calls.
         */
        explicit Socket(const NetworkBackend::Symbols &symbols);
        /**
         * @brief Wraps an already-accepted file descriptor as a client socket.
         * @param fd Connected client file descriptor.
         * @param symbols Backend function table.
         */
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
        void setTcpNoDelay(bool noDelay) override;

        int getSocket() const override;
        int getClientSocket() const override;

    private:
        const NetworkBackend::Symbols &_symbols; /**< Resolved backend calls. */
        int _socket;       /**< Listening/own socket fd. */
        int _port;         /**< Bound port. */
        int _backlog;      /**< Listen backlog. */
        int _clientSocket; /**< Last accepted client fd. */
        std::string _data; /**< Scratch receive buffer. */
};

#endif /* !SOCKET_HPP_ */