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

extern std::map<std::string, void (AiCommandParser::*)(std::shared_ptr<AI>, const std::string &command)> ai_cmds;

#endif /* !AI_EXECUTE_CMDS_HPP_ */
