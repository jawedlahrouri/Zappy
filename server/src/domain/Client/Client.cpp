/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.cpp
*/

#include "Client.hpp"

/**
 * @brief Takes ownership of the socket; the client starts as @c PENDING.
 */
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
    _writeBuffer += data;
}

std::string Client::getWriteBuffer() const
{
    return _writeBuffer;
}

bool Client::hasWriteData() const
{
    return !_writeBuffer.empty();
}

void Client::clearWriteBuffer()
{
    _writeBuffer.clear();
}

/**
 * @brief Drops the first @p len sent bytes, clearing fully if @p len covers all.
 */
void Client::removeFromWriteBuffer(size_t len)
{
    if (len >= _writeBuffer.size()) {
        _writeBuffer.clear();
    } else {
        _writeBuffer.erase(0, len);
    }
}
