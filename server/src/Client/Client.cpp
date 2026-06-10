/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.cpp
*/

#include "Client/Client.hpp"

Client::Client(Socket clientSocket)
    : socket(std::move(clientSocket)), type(ClientType::PENDING)
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
