/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** respond.cpp
*/

#include "Parsing/Respond.hpp"
#include "Server/Server.hpp"
#include "Exception_Server.hpp"


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
            it->second->append_to_write_buffer(msg_to_send);
            server.add_poll_event(client_socket, SysPoll::EventOut);
        }
    } catch (const std::exception &e) {
        server.get_logger().error(std::string("[ERROR] send_response exception: ") + e.what());
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
        server.get_logger().error(std::string("[ERROR] broadcast_event exception: ") + e.what());
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
        server.get_logger().error(std::string("[ERROR] send_to_guis exception: ") + e.what());
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
        server.get_logger().error(std::string("[ERROR] send_to_ai exception: ") + e.what());
    }
}
