/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.cpp
*/

#include "Socket/Socket.hpp"
#include "Socket/Sys_Socket.hpp"
#include "ServerException.hpp"
#include <vector>

Socket::Socket()
    : m_socket(-1), m_port(0), m_backlog(0), m_clientSocket(-1)
{
}

Socket::Socket(int fd)
    : m_socket(fd), m_port(0), m_backlog(0), m_clientSocket(-1)
{
}

void Socket::createSocket(int port, int backlog)
{
    m_socket = SysSocket::createInetSocket();
    if (m_socket == -1) {
        throw ServerException(std::string("Socket creation failed: ") + SysSocket::getLastError());
    }
    if (SysSocket::setReuseAddr(m_socket, true) == -1) {
        throw ServerException(std::string("setsockopt SO_REUSEADDR failed: ") + SysSocket::getLastError());
    }
    m_port = port;
    m_backlog = backlog;
    setNonBlocking(true);
}

void Socket::bindSocket()
{
    if (SysSocket::bindInet(m_socket, m_port) == -1) {
        throw ServerException(std::string("Socket binding failed: ") + SysSocket::getLastError());
    }
}

void Socket::receiveData()
{
    if (SysSocket::listen(m_socket, m_backlog) == -1) {
        throw ServerException(std::string("Socket listening failed: ") + SysSocket::getLastError());
    }
}

std::unique_ptr<ISocket> Socket::acceptConnection()
{
    int fd = SysSocket::acceptInet(m_socket);
    if (fd == -1) {
        throw ServerException(std::string("Socket acceptance failed: ") + SysSocket::getLastError());
    }
    return std::make_unique<Socket>(fd);
}

size_t Socket::sendData(const std::string &data)
{
    ssize_t sent = SysSocket::send(m_socket, data.c_str(), data.length());
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
    ssize_t bytes_read = SysSocket::recv(m_socket, buf.data(), maxSize);
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
    if (SysSocket::setNonBlocking(m_socket, nonBlocking) == -1) {
        throw ServerException(std::string("set non blocking failed: ") + SysSocket::getLastError());
    }
}

void Socket::closeSocket()
{
    if (m_clientSocket != -1) {
        SysSocket::close(m_clientSocket);
        m_clientSocket = -1;
    }
    if (m_socket != -1) {
        SysSocket::close(m_socket);
        m_socket = -1;
    }
}

int Socket::getSocket() const
{
    return m_socket;
}

int Socket::getClientSocket() const
{
    return m_clientSocket;
}