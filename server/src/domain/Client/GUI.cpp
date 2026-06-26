/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GUI.cpp
*/

#include "GUI.hpp"

/**
 * @brief Builds a GUI client and tags it as @c ClientType::GUI.
 */
GUI::GUI(std::unique_ptr<ISocket> clientSocket, int id)
    : Client(std::move(clientSocket), id)
{
    setType(ClientType::GUI);
}
