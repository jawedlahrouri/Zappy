/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GUI.cpp
*/

#include "Client/GUI.hpp"

GUI::GUI(Socket clientSocket)
    : Client(std::move(clientSocket))
{
    setType(ClientType::GUI);
}
