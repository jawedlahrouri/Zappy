/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ai_parsing.cpp
*/

#include "Parsing.hpp"
#include "application/Server/Server.hpp"
#include "shared/ILogger.hpp"
#include <sstream>
#include <algorithm>
#include "ai_execute_cmds.hpp"
#include "ServerException.hpp"
#include "AI.hpp"


/**
 * @brief Splits a command line into whitespace-separated tokens.
 */
std::vector<std::string> Parsing::split_command(const std::string &command)
{
    try {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(command);

        while (tokenStream >> token) {
            tokens.push_back(token);
        }
        return tokens;
    } catch (const std::exception &e) {
        throw ServerException(std::string("split_command failed: ") + e.what());
    }
}

/**
 * @brief Validates and queues an AI command for tick-paced execution.
 * @details Unknown commands get "ko". Valid ones are appended to the player's
 *          command queue, which is capped at 10 pending commands per the
 *          protocol; extra commands are dropped.
 */
void Parsing::parse_ai_command(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::vector<std::string> tokens = split_command(command);
        if (tokens.empty())
            return;

        std::string action = tokens[0];
        if (ai_cmds.find(action) == ai_cmds.end()) {
            network.sendResponse(client->getId(), "ko");
            return;
        }

        // queue 10 commands
        if (client->getCommandQueue().size() < 10) {
            client->getCommandQueue().push(command);
            network.getLogger().debug("AI [#" + std::to_string(client->getId()) + "] queued command: " + command);
        } else {
            network.getLogger().debug("AI [#" + std::to_string(client->getId()) + "] command queue full, command dropped");
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("parse_ai_command failed: ") + e.what());
    }
}
