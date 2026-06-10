/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Respond.hpp
*/

#ifndef RESPOND_HPP_
#define RESPOND_HPP_

#include <string>

class Server;

class Respond {
    public:
        static void send_response(Server &server, int client_socket, const std::string &message);
        static void broadcast_event(Server &server, const std::string &message, int exclude_socket = -1);
        static void send_to_guis(Server &server, const std::string &message);
        static void send_to_ai(Server &server, const std::string &message);
};

#endif /* !RESPOND_HPP_ */
