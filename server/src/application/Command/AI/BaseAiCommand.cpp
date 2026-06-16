/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BaseAiCommand.cpp
*/

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"

void BaseAiCommand::setClient(std::shared_ptr<AI> client) {
    _client = client;
}

std::shared_ptr<AI> BaseAiCommand::getClient() const {
    return _client;
}

int BaseAiCommand::getX() const {
    if (_client)
        return _client->getX();
    return -1;
}

int BaseAiCommand::getY() const {
    if (_client)
        return _client->getY();
    return -1;
}
