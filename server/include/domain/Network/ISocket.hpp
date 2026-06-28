/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ISocket.hpp
*/

/**
 * @file ISocket.hpp
 * @brief Abstraction over a TCP socket used by the server.
 */

#ifndef ISOCKET_HPP_
#define ISOCKET_HPP_

#include <string>
#include <memory>

/**
 * @interface ISocket
 * @brief Contract for the socket operations the server relies on.
 *
 * @par Design Pattern: abstract interface (Dependency Inversion)
 * Defines socket behaviour (creation, binding, accept, I/O) independently of the
 * underlying system calls. The concrete @ref Socket implementation forwards to a
 * dynamically loaded network backend, while tests can substitute a mock, so no
 * higher layer ever depends on @c <sys/socket.h> directly.
 */
class ISocket {
    public:
        virtual ~ISocket() = default;

        /**
         * @brief Creates the listening socket and starts listening.
         * @param port TCP port to listen on.
         * @param backlog Maximum length of the pending-connections queue.
         * @throws ServerException If the socket cannot be created.
         */
        virtual void createSocket(int port, int backlog) = 0;

        /**
         * @brief Binds the socket to its configured port.
         * @throws ServerException If binding fails.
         */
        virtual void bindSocket() = 0;

        /**
         * @brief Marks the socket ready to accept incoming connections.
         */
        virtual void receiveData() = 0;

        /**
         * @brief Accepts a pending incoming connection.
         * @return A new @ref ISocket for the connected client, or @c nullptr if none.
         */
        virtual std::unique_ptr<ISocket> acceptConnection() = 0;

        /**
         * @brief Sends data to the connected peer (non-blocking).
         * @param data Bytes to transmit.
         * @return Number of bytes actually written.
         */
        virtual size_t sendData(const std::string &data) = 0;

        /**
         * @brief Reads up to @p maxSize bytes from the socket (non-blocking).
         * @param maxSize Maximum number of bytes to read.
         * @return The received data; empty if nothing is available.
         */
        virtual std::string recvData(size_t maxSize) = 0;

        /**
         * @brief Enables or disables non-blocking mode.
         * @param nonBlocking @c true to make I/O non-blocking.
         */
        virtual void setNonBlocking(bool nonBlocking) = 0;

        /**
         * @brief Enables or disables Nagle's algorithm (TCP_NODELAY).
         * @param noDelay @c true to disable Nagle for lower latency.
         */
        virtual void setTcpNoDelay(bool noDelay) = 0;

        /**
         * @brief Closes the socket file descriptor.
         */
        virtual void closeSocket() = 0;

        /**
         * @brief Returns the listening/own socket file descriptor.
         * @return The file descriptor.
         */
        virtual int getSocket() const = 0;

        /**
         * @brief Returns the connected client socket file descriptor.
         * @return The client file descriptor.
         */
        virtual int getClientSocket() const = 0;
};

#endif /* !ISOCKET_HPP_ */
