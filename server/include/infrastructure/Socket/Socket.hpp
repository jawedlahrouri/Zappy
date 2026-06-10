/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Socket.hpp
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <netinet/in.h>
#include <string>

class Socket {
    public:
        Socket();
        Socket(int fd);
        ~Socket() = default;

        void create_socket(int port, int backlog);
        void close_socket();
        void bind_socket();
        Socket accept_connection();
        void receive_data();
        void send_data(const std::string &data);
        std::string recv_data(size_t max_size);

        int get_socket() const;
        int get_client_socket() const;

    private:
        int _socket;
        int _port;
        int _backlog;
        int _client_socket;
        std::string _data;
};

#endif /* !SOCKET_HPP_ */