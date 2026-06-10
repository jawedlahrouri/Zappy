/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.cpp
*/

#include "Socket/Socket.hpp"
#include "Socket/Sys_Socket.hpp"
#include "Exception_Server.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <cerrno>

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
    _socket = SysSocket::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw Exception_Server(std::string("Socket creation failed: ") + std::strerror(errno));
    }
    _port = port;
    _backlog = backlog;
}

void Socket::bind_socket()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (SysSocket::bind(_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        throw Exception_Server(std::string("Socket binding failed: ") + std::strerror(errno));
    }
}

void Socket::receive_data()
{
    if (SysSocket::listen(_socket, _backlog) == -1) {
        throw Exception_Server(std::string("Socket listening failed: ") + std::strerror(errno));
    }
}

Socket Socket::accept_connection()
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int fd = SysSocket::accept(_socket, (struct sockaddr *)&addr, &addr_len);
    if (fd == -1) {
        throw Exception_Server(std::string("Socket acceptance failed: ") + std::strerror(errno));
    }
    return Socket(fd);
}

void Socket::send_data(const std::string &data)
{
    if (SysSocket::send(_socket, data.c_str(), data.length(), MSG_NOSIGNAL) == -1) {
        throw Exception_Server(std::string("Socket sending failed: ") + std::strerror(errno));
    }
}

std::string Socket::recv_data(size_t max_size)
{
    std::vector<char> buf(max_size + 1, 0);
    ssize_t bytes_read = SysSocket::recv(_socket, buf.data(), max_size, 0);
    if (bytes_read <= 0) {
        return "";
    }
    return std::string(buf.data(), bytes_read);
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

int Socket::get_socket() const
{
    return _socket;
}

int Socket::get_client_socket() const
{
    return _client_socket;
}