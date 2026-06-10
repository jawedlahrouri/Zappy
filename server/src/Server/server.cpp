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
#include <chrono>
#include "Parsing/AiCommandParser.hpp"
#include "Parsing/ai_execute_cmds.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Event/GUI/EventVisitor.hpp"
#include "Event/GUI/PdiEvent.hpp"
#include "Parsing/Respond.hpp"
#include "Server/login.hpp"



Server::Server() = default;

Server::~Server()
{
    try {
        cleanup();
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << Exception_Server(std::string("Server::~Server: ") + e.what()).what() << std::endl;
    }
}

void Server::send_event(const IEventElement &event)
{
    try {
    EventVisitor visitor(*this);
    event.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::send_event: ") + e.what());
    }
}

void Server::detect_client_type(int client_socket)
{
    try {
        auto it = _clients.find(client_socket);
        std::string line;
        bool team_exists = false;
        
        if (it == _clients.end() || it->second->type != ClientType::PENDING)
            return;

        std::shared_ptr<Client> &client = it->second;
        if (!client->extract_line(line))
            return;
        if (line == "GRAPHIC") {
            auto gui_client = std::make_shared<GUI>(std::move(client->socket));
            _clients[client_socket] = gui_client;
            Login::login_gui(gui_client, *this);
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
                Login::login_ai(ai_client, line, *this);
                std::cout << "Client " << client_socket << " authenticated as AI in team " << line << std::endl;
            } else {
                Respond::send_response(*this, client_socket, "ko");
                remove_client(client_socket);
            }
        }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::detect_client_type: ") + e.what());
    }
}

void Server::accept_client()
{
    try {
        Socket client_socket = _master_socket.accept_connection();
        int fd = client_socket.get_socket();
        
        if (fd != -1) {
            _poll.add_fd(fd, SysPoll::EventIn);
            _clients[fd] = std::make_shared<Client>(std::move(client_socket));
            std::cout << "New client connected on socket " << fd << std::endl;
            Respond::send_response(*this, fd, "WELCOME");
        }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::accept_client: ") + e.what());
    }
}

void Server::remove_client(int client_socket)
{
    try {
        auto it = _clients.find(client_socket);
        if (it != _clients.end()) {
            if (it->second->type == ClientType::AI) {
                auto ai = std::dynamic_pointer_cast<AI>(it->second);
                if (ai)
                    send_event(PdiEvent(ai->id));
            }
            if (!it->second->get_write_buffer().empty()) {
                try {
                    it->second->socket.send_data(it->second->get_write_buffer());
                } catch (...) {}
            }
            it->second->socket.close_socket();
            _clients.erase(it);
        }
        _poll.remove_fd(client_socket);
        std::cout << "Client " << client_socket << " disconnected" << std::endl;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::remove_client: ") + e.what());
    }
}

bool Server::handle_writable(size_t index)
{
    try {
        int client_socket = _poll.get_fd(index);
        auto it = _clients.find(client_socket);
        if (it == _clients.end()) {
            _poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

        const auto &buffer = it->second->get_write_buffer();
        if (buffer.empty()) {
            _poll.remove_events(client_socket, SysPoll::EventOut);
            return false;
        }

    try {
        it->second->socket.send_data(buffer);
        it->second->clear_write_buffer();
        _poll.remove_events(client_socket, SysPoll::EventOut);
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << Exception_Server(std::string("handle_writable: ") + e.what()).what() << std::endl;
        remove_client(client_socket);
        return true;
    }
    return false;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::handle_writable: ") + e.what());
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
            it->second->append_to_buffer(data);
            if (it->second->type == ClientType::PENDING) {
                detect_client_type(client_socket);
            } else {
                std::string line;
                while (it->second->extract_line(line)) {
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
        throw Exception_Server(std::string("Server::handle_client_data: ") + e.what());
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
        throw Exception_Server(std::string("Server::cleanup: ") + e.what());
    }
}

void Server::process_poll_event(size_t &i)
{
    try {
        if (_poll.is_error(i)) {
            if (i > 0) {
                remove_client(_poll.get_fd(i));
                --i;
            }
            return;
        }
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
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::process_poll_event: ") + e.what());
    }
}

void Server::run_poll_loop()
{
    try {
        std::cout << "Waiting for connections..." << std::endl;
        std::chrono::microseconds tick_duration = std::chrono::microseconds(1000000 / _freq);
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
                if (_game_engine) {
                    _game_engine->update(*this);
                }
                next_tick_time += tick_duration;
            }
        }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("run_poll_loop: ") + e.what());
    }
}

void Server::open_server(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq, int width, int height)
{
    try {
    _port = port;
    _clientsNb = clientsNb;
    _teamNames = teamNames;
    _freq = freq;
    try {
        _master_socket.create_socket(port, clientsNb);
        _master_socket.bind_socket();
        _master_socket.receive_data();

        std::cout << "Server opened on port " << _port << std::endl;
        _poll.add_fd(_master_socket.get_socket(), SysPoll::EventIn);

        _game_engine = std::make_unique<GameEngine>(width, height);

        run_poll_loop();
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("open_server: ") + e.what());
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Server::open_server: ") + e.what());
    }
}