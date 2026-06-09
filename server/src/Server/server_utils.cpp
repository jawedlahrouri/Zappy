/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** server_utils.cpp
*/

#include "Server/Server.hpp"

Client::Client(Socket clientSocket)
    : socket(clientSocket), type(ClientType::PENDING)
{
}

void Client::append_to_buffer(const std::string& data)
{
    _buffer += data;
}

bool Client::extract_line(std::string& line)
{
    size_t newline_pos = _buffer.find('\n');
    if (newline_pos == std::string::npos)
        return false;
    line = _buffer.substr(0, newline_pos);
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    _buffer.erase(0, newline_pos + 1);
    return true;
}

void Client::append_to_write_buffer(const std::string& data)
{
    _write_buffer += data;
}

const std::string& Client::get_write_buffer() const
{
    return _write_buffer;
}

void Client::clear_write_buffer()
{
    _write_buffer.clear();
}

AI::AI(Socket clientSocket, int player_id)
    : Client(clientSocket), id(player_id), level(1), orientation(1), food_ticks(126)
{
    type = ClientType::AI;
}

void AI::update(std::shared_ptr<Client> self, Server& server, GameEngine& engine, std::vector<int>& clients_to_remove)
{
    engine.update_ai(std::dynamic_pointer_cast<AI>(self), server, clients_to_remove);
}

GUI::GUI(Socket clientSocket)
    : Client(clientSocket)
{
    type = ClientType::GUI;
}
