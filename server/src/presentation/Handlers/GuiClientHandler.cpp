/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GuiClientHandler.cpp
*/

#include "presentation/Handlers/GuiClientHandler.hpp"
#include "application/Server/Server.hpp"
#include "presentation/Parsing/Parsing.hpp"
#include "GUI.hpp"

/**
 * @brief Parses and immediately executes every complete line from a GUI client.
 */
void GuiClientHandler::handleData(std::shared_ptr<Client> client, Server& server)
{
    std::string line;
    while (client->extractLine(line)) {
        auto gui_client = std::dynamic_pointer_cast<GUI>(client);
        if (gui_client) {
            Parsing::parse_gui_command(gui_client, line, server);
        }
    }
}
