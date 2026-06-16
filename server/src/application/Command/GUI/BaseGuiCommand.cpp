/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BaseGuiCommand.cpp
*/

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include "domain/Client/GUI.hpp"

void BaseGuiCommand::setClient(std::shared_ptr<GUI> client) {
    _client = client;
}

std::shared_ptr<GUI> BaseGuiCommand::getClient() const {
    return _client;
}
