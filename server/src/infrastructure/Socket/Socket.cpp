/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.cpp
*/

#include "Socket.hpp"
#include "Sys_Socket.hpp"
#include "ServerException.hpp"

#include <vector>

Socket::Socket(const NetworkBackend::Symbols &symbols)
    : _symbols(symbols), _socket(-1), _port(0), _backlog(0), _clientSocket(-1)
{
}

Socket::Socket(int fd, const NetworkBackend::Symbols &symbols)
    : _symbols(symbols), _socket(fd), _port(0), _backlog(0), _clientSocket(-1)
{
}

/**
 * @brief Creates the socket, enables SO_REUSEADDR and sets it non-blocking.
 * @details Stores @p port / @p backlog for the later @ref bindSocket and
 *          @ref receiveData calls.
 */
void Socket::createSocket(int port, int backlog)
{
    _socket = _symbols.createInetSocket();
    if (_socket == -1) {
        throw ServerException(std::string("Socket creation failed: ") + _symbols.getLastError());
    }
    int reuse = 1;
    if (_symbols.setReuseAddr(_socket, reuse) == -1) {
        throw ServerException(std::string("setsockopt SO_REUSEADDR failed: ") + _symbols.getLastError());
    }
    _port = port;
    _backlog = backlog;
    setNonBlocking(true);
}

void Socket::bindSocket()
{
    if (_symbols.bindInet(_socket, _port) == -1) {
        throw ServerException(std::string("Socket binding failed: ") + _symbols.getLastError());
    }
}

void Socket::receiveData()
{
    if (_symbols.listen(_socket, _backlog) == -1) {
        throw ServerException(std::string("Socket listening failed: ") + _symbols.getLastError());
    }
}

/**
 * @brief Accepts a pending connection, wrapping it in a new client @ref Socket.
 * @return The client socket, or @c nullptr when no connection is pending
 *         (EWOULDBLOCK on the non-blocking listening socket).
 */
std::unique_ptr<ISocket> Socket::acceptConnection()
{
    int fd = _symbols.acceptInet(_socket);
    if (fd == -1) {
        if (_symbols.wouldBlock()) {
            return nullptr;
        }
        throw ServerException(std::string("Socket acceptance failed: ") + _symbols.getLastError());
    }
    return std::make_unique<Socket>(fd, _symbols);
}

/**
 * @brief Sends bytes without blocking.
 * @return Bytes written, or @c 0 if the send would block (retry later).
 */
size_t Socket::sendData(const std::string &data)
{
    long sent = _symbols.send(_socket, data.c_str(), data.length());
    if (sent == -1) {
        if (_symbols.wouldBlock()) {
            return 0;
        }
        throw ServerException(std::string("Socket sending failed: ") + _symbols.getLastError());
    }
    return static_cast<size_t>(sent);
}

/**
 * @brief Reads up to @p maxSize bytes without blocking.
 * @return The bytes read; @c "" if the read would block.
 * @throws ServerException If the peer closed the connection (recv == 0) or on
 *         a genuine socket error.
 */
std::string Socket::recvData(size_t maxSize)
{
    std::vector<char> buf(maxSize + 1, 0);
    long bytesRead = _symbols.recv(_socket, buf.data(), maxSize);
    if (bytesRead == 0) {
        throw ServerException("Connection closed by peer");
    }
    if (bytesRead == -1) {
        if (_symbols.wouldBlock()) {
            return "";
        }
        throw ServerException(std::string("Socket recv failed: ") + _symbols.getLastError());
    }
    return std::string(buf.data(), static_cast<size_t>(bytesRead));
}

void Socket::setNonBlocking(bool nonBlocking)
{
    int flag = nonBlocking ? 1 : 0;
    if (_symbols.setNonBlocking(_socket, flag) == -1) {
        throw ServerException(std::string("set non blocking failed: ") + _symbols.getLastError());
    }
}

void Socket::setTcpNoDelay(bool noDelay)
{
    if (SysSocket::setTcpNoDelay(_socket, noDelay) == -1) {
        throw ServerException(std::string("set tcp no delay failed: ") + SysSocket::getLastError());
    }
}

/**
 * @brief Closes the client fd (if any) then the own/listening fd.
 */
void Socket::closeSocket()
{
    if (_clientSocket != -1) {
        _symbols.close(_clientSocket);
        _clientSocket = -1;
    }
    if (_socket != -1) {
        _symbols.close(_socket);
        _socket = -1;
    }
}

int Socket::getSocket() const
{
    return _socket;
}

int Socket::getClientSocket() const
{
    return _clientSocket;
}