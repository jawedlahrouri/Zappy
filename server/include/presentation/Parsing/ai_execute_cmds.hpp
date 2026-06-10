/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ai_execute_cmds.hpp
*/

#ifndef AI_EXECUTE_CMDS_HPP_
#define AI_EXECUTE_CMDS_HPP_

#include <map>
#include <string>
#include <memory>

class AiCommandParser;
class AI;
class Server;

struct AiCommand {
    void (AiCommandParser::*func)(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
    int duration;
};

extern std::map<std::string, AiCommand> ai_cmds;

#endif /* !AI_EXECUTE_CMDS_HPP_ */
