/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AiClientHandler.cpp
*/

#include "presentation/Handlers/AiClientHandler.hpp"
#include "application/Server/Server.hpp"
#include "presentation/Parsing/Parsing.hpp"
#include "AI.hpp"

/**
 * @brief Parses every complete line received from an AI client.
 * @details Each line is routed through @ref Parsing::parse_ai_command, which
 *          enqueues the command for tick-paced execution by the game engine.
 */
void AiClientHandler::handleData(std::shared_ptr<Client> client, Server& server)
{
    std::string line;
    while (client->extractLine(line)) {
        auto ai_client = std::dynamic_pointer_cast<AI>(client);
        if (ai_client) {
            Parsing::parse_ai_command(ai_client, line, server);
        }
    }
}
