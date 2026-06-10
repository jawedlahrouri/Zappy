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
    _read_buffer.append(data);
}

bool Client::extract_line(std::string& line)
{
    return _read_buffer.extract_line(line);
}

void Client::append_to_write_buffer(const std::string& data)
{
    _write_buffer.append(data);
}

std::string Client::get_write_buffer() const
{
    return _write_buffer.peek_all();
}

bool Client::has_write_data() const
{
    return !_write_buffer.is_empty();
}

void Client::clear_write_buffer()
{
    _write_buffer.clear();
}

void Client::remove_from_write_buffer(size_t len)
{
    _write_buffer.pop(len);
}
