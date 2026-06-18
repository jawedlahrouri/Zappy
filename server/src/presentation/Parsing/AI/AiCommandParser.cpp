/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AiCommandParser.cpp
*/

#include "presentation/Parsing/AiCommandParser.hpp"
#include "presentation/Parsing/ai_execute_cmds.hpp"
#include "ServerException.hpp"

void AiCommandParser::executeCommand(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network)
{
    try {
        std::string action = command.substr(0, command.find(' '));
        auto it = ai_cmds.find(action);
        if (it != ai_cmds.end()) {
            auto func = it->second.func;
            (this->*func)(client, command, network);
        } else {
            network.sendResponse(client->getId(), "ko\n");
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("AiCommandParser::executeCommand: ") + e.what());
    }
}
