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

/**
 * @struct AiCommand
 * @brief Dispatch-table entry: parser member pointer + tick duration.
 *
 * @par Design Pattern: Command (table-driven dispatch)
 * Maps a command name to a member-function pointer on @ref AiCommandParser and
 * the action's tick cost, so @c executeCommand and @c getCommandDuration are
 * just table lookups instead of long if/else chains.
 */
struct AiCommand {
    /** @brief Parser method handling this command. */
    void (AiCommandParser::*func)(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
    int duration; /**< Action cost in time units (divided by the frequency). */
};

/** @brief Command name -> @ref AiCommand table (defined in ai_execute_cmds.cpp). */
extern std::map<std::string, AiCommand> ai_cmds;

#endif /* !AI_EXECUTE_CMDS_HPP_ */
