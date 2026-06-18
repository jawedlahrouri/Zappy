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

Socket::Socket()
    : _socket(-1), _port(0), _backlog(0), _clientSocket(-1)
{
}

Socket::Socket(int fd)
    : _socket(fd), _port(0), _backlog(0), _clientSocket(-1)
{
}

void Socket::createSocket(int port, int backlog)
{
    _socket = SysSocket::createInetSocket();
    if (_socket == -1) {
        throw ServerException(std::string("Socket creation failed: ") + SysSocket::getLastError());
    }
    if (SysSocket::setReuseAddr(_socket, true) == -1) {
        throw ServerException(std::string("setsockopt SO_REUSEADDR failed: ") + SysSocket::getLastError());
    }
    _port = port;
    _backlog = backlog;
    setNonBlocking(true);
}

void Socket::bindSocket()
{
    if (SysSocket::bindInet(_socket, _port) == -1) {
        throw ServerException(std::string("Socket binding failed: ") + SysSocket::getLastError());
    }
}

void Socket::receiveData()
{
    if (SysSocket::listen(_socket, _backlog) == -1) {
        throw ServerException(std::string("Socket listening failed: ") + SysSocket::getLastError());
    }
}

std::unique_ptr<ISocket> Socket::acceptConnection()
{
    int fd = SysSocket::acceptInet(_socket);
    if (fd == -1) {
        if (SysSocket::wouldBlock()) {
            return nullptr;
        }
        throw ServerException(std::string("Socket acceptance failed: ") + SysSocket::getLastError());
    }
    return std::make_unique<Socket>(fd);
}

size_t Socket::sendData(const std::string &data)
{
    ssize_t sent = SysSocket::send(_socket, data.c_str(), data.length());
    if (sent == -1) {
        if (SysSocket::wouldBlock()) {
            return 0;
        }
        throw ServerException(std::string("Socket sending failed: ") + SysSocket::getLastError());
    }
    return static_cast<size_t>(sent);
}

std::string Socket::recvData(size_t maxSize)
{
    std::vector<char> buf(maxSize + 1, 0);
    ssize_t bytes_read = SysSocket::recv(_socket, buf.data(), maxSize);
    if (bytes_read == 0) {
        throw ServerException("Connection closed by peer");
    }
    if (bytes_read == -1) {
        if (SysSocket::wouldBlock()) {
            return "";
        }
        throw ServerException(std::string("Socket recv failed: ") + SysSocket::getLastError());
    }
    return std::string(buf.data(), bytes_read);
}

void Socket::setNonBlocking(bool nonBlocking)
{
    if (SysSocket::setNonBlocking(_socket, nonBlocking) == -1) {
        throw ServerException(std::string("set non blocking failed: ") + SysSocket::getLastError());
    }
}

void Socket::setTcpNoDelay(bool noDelay)
{
    if (SysSocket::setTcpNoDelay(_socket, noDelay) == -1) {
        throw ServerException(std::string("set tcp no delay failed: ") + SysSocket::getLastError());
    }
}

void Socket::closeSocket()
{
    if (_clientSocket != -1) {
        SysSocket::close(_clientSocket);
        _clientSocket = -1;
    }
    if (_socket != -1) {
        SysSocket::close(_socket);
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