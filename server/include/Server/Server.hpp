/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.hpp
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include "Socket/Socket.hpp"
#include "Poll/Poll.hpp"

class Server {
    public:
        Server() = default;
        ~Server();

        void open_server(int port, int clientsNb, const std::vector<std::string> &teamNames);

    private:
        void run_poll_loop();
        void process_poll_event(size_t &i);
        void accept_client();
        void remove_client(int client_socket);
        bool handle_client_data(size_t poll_index);
        bool handle_writable(size_t index);
        void send_response(int client_socket, const std::string &message);
        void broadcast_event(const std::string &message, int exclude_socket = -1);
        void cleanup();

        Socket _master_socket;
        Poll _poll;
        std::unordered_map<int, Socket> _clients;
        std::unordered_map<int, std::string> _client_buffers;
        std::unordered_map<int, std::string> _output_buffers;

        int _port = 0;
        int _clientsNb = 0;
        std::vector<std::string> _teamNames;
};

#endif /* !SERVER_HPP_ */