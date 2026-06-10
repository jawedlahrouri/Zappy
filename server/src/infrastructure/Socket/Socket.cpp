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
    : _socket(-1), _port(0), _backlog(0), _client_socket(-1)
{
}

Socket::Socket(int fd)
    : _socket(fd), _port(0), _backlog(0), _client_socket(-1)
{
}

void Socket::create_socket(int port, int backlog)
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
    set_non_blocking(true);
}

void Socket::bind_socket()
{
    if (SysSocket::bindInet(_socket, _port) == -1) {
        throw ServerException(std::string("Socket binding failed: ") + SysSocket::getLastError());
    }
}

void Socket::receive_data()
{
    if (SysSocket::listen(_socket, _backlog) == -1) {
        throw ServerException(std::string("Socket listening failed: ") + SysSocket::getLastError());
    }
}

Socket Socket::accept_connection()
{
    int fd = SysSocket::acceptInet(_socket);
    if (fd == -1) {
        throw ServerException(std::string("Socket acceptance failed: ") + SysSocket::getLastError());
    }
    return Socket(fd);
}

size_t Socket::send_data(const std::string &data)
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

std::string Socket::recv_data(size_t max_size)
{
    std::vector<char> buf(max_size + 1, 0);
    ssize_t bytes_read = SysSocket::recv(_socket, buf.data(), max_size);
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

void Socket::set_non_blocking(bool non_blocking)
{
    if (SysSocket::setNonBlocking(_socket, non_blocking) == -1) {
        throw ServerException(std::string("set non blocking failed: ") + SysSocket::getLastError());
    }
}

void Socket::close_socket()
{
    if (_client_socket != -1) {
        SysSocket::close(_client_socket);
        _client_socket = -1;
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

int Socket::get_client_socket() const
{
    return _client_socket;
}