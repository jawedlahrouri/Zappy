/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.cpp
*/

#include "Client/Client.hpp"

Client::Client(std::unique_ptr<ISocket> clientSocket, int id)
    : m_socket(std::move(clientSocket)), m_id(id), m_type(ClientType::PENDING)
{
}

void Client::appendToBuffer(const std::string& data)
{
    m_readBuffer.append(data);
}

bool Client::extractLine(std::string& line)
{
    return m_readBuffer.extract_line(line);
}

void Client::appendToWriteBuffer(const std::string& data)
{
    m_writeBuffer.append(data);
}

std::string Client::getWriteBuffer() const
{
    return m_writeBuffer.peek_all();
}

bool Client::hasWriteData() const
{
    return !m_writeBuffer.is_empty();
}

void Client::clearWriteBuffer()
{
    m_writeBuffer.clear();
}

void Client::removeFromWriteBuffer(size_t len)
{
    m_writeBuffer.pop(len);
}
