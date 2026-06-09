/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** respond.cpp
*/

#include "Parsing/Respond.hpp"
#include "Server/Server.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void Respond::send_response(Server &server, int client_socket, const std::string &message)
{
    try {
        std::string msg_to_send = message;
        if (msg_to_send.empty() || msg_to_send.back() != '\n') {
            msg_to_send += "\n";
        }
        const auto &clients = server.get_clients();
        auto it = clients.find(client_socket);
        if (it != clients.end()) {
            it->second->write_buffer += msg_to_send;
            server.add_poll_event(client_socket, POLLOUT);
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] send_response exception: " << e.what() << std::endl;
    }
}

void Respond::broadcast_event(Server &server, const std::string &message, int exclude_socket)
{
    try {
        for (const auto &[socket, client] : server.get_clients()) {
            if (socket != exclude_socket) {
                send_response(server, socket, message);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] broadcast_event exception: " << e.what() << std::endl;
    }
}

void Respond::send_to_guis(Server &server, const std::string &message)
{
    try {
        for (const auto &[fd, client] : server.get_clients()) {
            if (client->type == ClientType::GUI) {
                send_response(server, fd, message);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] send_to_guis exception: " << e.what() << std::endl;
    }
}

void Respond::send_to_ai(Server &server, const std::string &message)
{
    try {
        for (const auto &[fd, client] : server.get_clients()) {
            if (client->type == ClientType::AI) {
                send_response(server, fd, message);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] send_to_ai exception: " << e.what() << std::endl;
    }
}
