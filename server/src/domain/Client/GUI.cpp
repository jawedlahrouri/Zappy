/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GUI.cpp
*/

#include "GUI.hpp"

GUI::GUI(std::unique_ptr<ISocket> clientSocket, int id)
    : Client(std::move(clientSocket), id)
{
    setType(ClientType::GUI);
}
