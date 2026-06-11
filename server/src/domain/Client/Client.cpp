/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.cpp
*/

#include "Client/Client.hpp"

Client::Client(std::unique_ptr<ISocket> clientSocket, int id)
    : _socket(std::move(clientSocket)), _id(id), _type(ClientType::PENDING)
{
}

void Client::appendToBuffer(const std::string& data)
{
    _readBuffer.append(data);
}

bool Client::extractLine(std::string& line)
{
    return _readBuffer.extract_line(line);
}

void Client::appendToWriteBuffer(const std::string& data)
{
    _writeBuffer.append(data);
}

std::string Client::getWriteBuffer() const
{
    return _writeBuffer.peek_all();
}

bool Client::hasWriteData() const
{
    return !_writeBuffer.is_empty();
}

void Client::clearWriteBuffer()
{
    _writeBuffer.clear();
}

void Client::removeFromWriteBuffer(size_t len)
{
    _writeBuffer.pop(len);
}
