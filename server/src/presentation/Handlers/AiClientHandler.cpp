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
