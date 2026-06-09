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
    try {
    _socket = SysSocket::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw Exception_Server("Socket creation failed");
    }
    _port = port;
    _backlog = backlog;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::create_socket: ") + e.what());
    }
}

void Socket::bind_socket()
{
    try {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (SysSocket::bind(_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        throw Exception_Server("Socket binding failed");
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::bind_socket: ") + e.what());
    }
}

void Socket::receive_data()
{
    try {
    if (SysSocket::listen(_socket, _backlog) == -1) {
        throw Exception_Server("Socket listening failed");
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::receive_data: ") + e.what());
    }
}

Socket Socket::accept_connection()
{
    try {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int fd = SysSocket::accept(_socket, (struct sockaddr *)&addr, &addr_len);
    if (fd == -1) {
        throw Exception_Server("Socket acceptance failed");
    }
    return Socket(fd);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::accept_connection: ") + e.what());
    }
}

void Socket::send_data(const std::string &data)
{
    try {
    if (SysSocket::send(_socket, data.c_str(), data.length(), 0) == -1) {
        throw Exception_Server("Socket sending failed");
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::send_data: ") + e.what());
    }
}

std::string Socket::recv_data(size_t max_size)
{
    try {
    std::vector<char> buf(max_size + 1, 0);
    ssize_t bytes_read = SysSocket::recv(_socket, buf.data(), max_size, 0);
    if (bytes_read <= 0) {
        return "";
    }
    return std::string(buf.data(), bytes_read);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::recv_data: ") + e.what());
    }
}

void Socket::close_socket()
{
    try {
    if (_client_socket != -1) {
        SysSocket::close(_client_socket);
        _client_socket = -1;
    }
    if (_socket != -1) {
        SysSocket::close(_socket);
        _socket = -1;
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::close_socket: ") + e.what());
    }
}

int Socket::get_socket() const
{
    try {
    return _socket;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::get_socket: ") + e.what());
    }
}

int Socket::get_client_socket() const
{
    try {
    return _client_socket;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Socket::get_client_socket: ") + e.what());
    }
}