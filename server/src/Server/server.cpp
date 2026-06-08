/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** server.cpp
*/

#include "Server/Server.hpp"
#include <iostream>
#include "Exception_Server.hpp"
#include "Socket/Socket.hpp"
#include "Poll/Poll.hpp"
#include <unistd.h>
#include <cstring>
#include "Parsing/Parsing.hpp"
#include "Clock_frequency/clock.hpp"
#include <chrono>

Client::Client(Socket clientSocket)
    : socket(clientSocket), type(ClientType::PENDING)
{
}

AI::AI(Socket clientSocket, int player_id)
    : Client(clientSocket), id(player_id), level(1), orientation(1), food_ticks(126)
{
    type = ClientType::AI;
}

GUI::GUI(Socket clientSocket)
    : Client(clientSocket)
{
    type = ClientType::GUI;
}

Server::~Server()
{
    cleanup();
}

bool Server::get_next_line(std::string &buffer, std::string &line)
{
    size_t newline_pos = buffer.find('\n');
    if (newline_pos == std::string::npos)
        return false;

    line = buffer.substr(0, newline_pos);
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    buffer.erase(0, newline_pos + 1);
    return true;
}

void Server::detect_client_type(int client_socket)
{
    auto it = _clients.find(client_socket);
    std::string line;
    bool team_exists = false;
    
    if (it == _clients.end() || it->second->type != ClientType::PENDING)
        return;

    std::shared_ptr<Client> &client = it->second;
    if (!get_next_line(client->buffer, line))
        return;
    if (line == "GRAPHIC") {
        _clients[client_socket] = std::make_shared<GUI>(std::move(client->socket));
        std::cout << "Client " << client_socket << " authenticated as GUI" << std::endl;
    } else {
        for (const auto &team : _teamNames) {
            if (line == team) {
                team_exists = true;
                break;
            }
        }
        if (team_exists) {
            int player_id = client_socket;
            auto ai_client = std::make_shared<AI>(std::move(client->socket), player_id);
            ai_client->teamName = line;
            _clients[client_socket] = ai_client;
            std::cout << "Client " << client_socket << " authenticated as AI in team " << line << std::endl;
        } else {
            send_response(client_socket, "ko");
            remove_client(client_socket);
        }
    }
}

void Server::accept_client()
{
    try {
        Socket client_socket = _master_socket.accept_connection();
        int fd = client_socket.get_socket();
        
        if (fd != -1) {
            _poll.add_fd(fd, POLLIN);
            _clients[fd] = std::make_shared<Client>(std::move(client_socket));
            std::cout << "New client connected on socket " << fd << std::endl;
            send_response(fd, "WELCOME");
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] accept_client exception: " << e.what() << std::endl;
    }
}

void Server::send_response(int client_socket, const std::string &message)
{
    try {
        std::string msg_to_send = message;
        if (msg_to_send.empty() || msg_to_send.back() != '\n') {
            msg_to_send += "\n";
        }
        auto it = _clients.find(client_socket);
        if (it != _clients.end()) {
            it->second->write_buffer += msg_to_send;
            _poll.add_events(client_socket, POLLOUT);
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] send_response exception: " << e.what() << std::endl;
    }
}

void Server::broadcast_event(const std::string &message, int exclude_socket)
{
    try {
        for (const auto &[socket, client] : _clients) {
            if (socket != exclude_socket) {
                send_response(socket, message);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] broadcast_event exception: " << e.what() << std::endl;
    }
}

void Server::send_to_guis(const std::string &message)
{
    try {
        for (const auto &[fd, client] : _clients) {
            if (client->type == ClientType::GUI) {
                send_response(fd, message);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "[ERROR] send_to_guis exception: " << e.what() << std::endl;
    }
}

void Server::remove_client(int client_socket)
{
    try {
        auto it = _clients.find(client_socket);
        if (it != _clients.end()) {
            it->second->socket.close_socket();
            _clients.erase(it);
        }
        _poll.remove_fd(client_socket);
        std::cout << "Client " << client_socket << " disconnected" << std::endl;
    } catch (const std::exception &e) {
        std::cout << "[ERROR] remove_client exception: " << e.what() << std::endl;
    }
}

bool Server::handle_writable(size_t index)
{
    try {
        int client_socket = _poll.get_fd(index);
        auto it = _clients.find(client_socket);
        if (it == _clients.end()) {
            _poll.remove_events(client_socket, POLLOUT);
            return false;
        }

        auto &buffer = it->second->write_buffer;
        if (buffer.empty()) {
            _poll.remove_events(client_socket, POLLOUT);
            return false;
        }

        it->second->socket.send_data(buffer);
        buffer.clear();
        _poll.remove_events(client_socket, POLLOUT);
        return false;
    } catch (const std::exception &e) {
        std::cout << "[ERROR] handle_writable exception: " << e.what() << std::endl;
        remove_client(_poll.get_fd(index));
        return true;
    }
}

bool Server::handle_client_data(size_t poll_index)
{
    try {
        int client_socket = _poll.get_fd(poll_index);
        auto it = _clients.find(client_socket);
        if (it == _clients.end()) {
            return false;
        }
        std::string data = it->second->socket.recv_data(1024);

        if (data.empty()) {
            remove_client(client_socket);
            return true;
        } else {
            it->second->buffer += data;
            if (it->second->type == ClientType::PENDING) {
                detect_client_type(client_socket);
            } else {
                std::string line;
                while (get_next_line(it->second->buffer, line)) {
                    if (it->second->type == ClientType::GUI) {
                        auto gui_client = std::dynamic_pointer_cast<GUI>(it->second);
                        Parsing::parse_gui_command(gui_client, line, *this);
                    } else if (it->second->type == ClientType::AI) {
                        auto ai_client = std::dynamic_pointer_cast<AI>(it->second);
                        Parsing::parse_ai_command(ai_client, line, *this);
                    }
                }
            }
        }
        return false;
    } catch (const std::exception &e) {
        std::cout << "[ERROR] handle_client_data exception: " << e.what() << std::endl;
        remove_client(_poll.get_fd(poll_index));
        return true;
    }
}

void Server::cleanup()
{
    try {
        for (auto &[fd, client] : _clients) {
            client->socket.close_socket();
        }
        _clients.clear();
        _master_socket.close_socket();
    } catch (const std::exception &e) {
        std::cout << "[ERROR] cleanup exception: " << e.what() << std::endl;
    }
}

void Server::process_poll_event(size_t &i)
{
    if (_poll.is_readable(i)) {
        if (i == 0) {
            accept_client();
        } else {
            if (handle_client_data(i)) {
                --i;
                return;
            }
        }
    }
    if (i > 0 && i < _poll.get_size() && _poll.is_writable(i)) {
        if (handle_writable(i)) {
            --i;
        }
    }
}

void Server::run_poll_loop(ClockFrequency &clock)
{
    std::cout << "Waiting for connections..." << std::endl;
    double tick_duration_ms = 1000.0 / clock.getClockFrequency();
    auto next_tick_time = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();
        int timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(next_tick_time - now).count();
        if (timeout_ms < 0) {
            timeout_ms = 0;
        }

        int poll_count = _poll.run_poll(timeout_ms);
        if (poll_count == -1) {
            if (errno == EINTR) {
                continue;
            }
            throw Exception_Server("poll failed");
        }

        for (size_t i = 0; i < _poll.get_size(); ++i) {
            process_poll_event(i);
        }

        now = std::chrono::steady_clock::now();
        if (now >= next_tick_time) {
            clock.tick();
            next_tick_time += std::chrono::milliseconds(static_cast<int>(tick_duration_ms));
        }
    }
}

void Server::open_server(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq)
{
    _port = port;
    _clientsNb = clientsNb;
    _teamNames = teamNames;
    try {
        _master_socket.create_socket(port, clientsNb);
        _master_socket.bind_socket();
        _master_socket.receive_data();

        std::cout << "Server opened on port " << _port << std::endl;
        _poll.add_fd(_master_socket.get_socket(), POLLIN);

        ClockFrequency clock(freq);
        run_poll_loop(clock);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("open_server: ") + e.what());
    }
}