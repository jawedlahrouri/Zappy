/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** gui_execute_cmds.hpp
*/

#ifndef GUI_EXECUTE_CMDS_HPP_
#define GUI_EXECUTE_CMDS_HPP_

#include <map>
#include <string>
#include <memory>

class GuiCommandParser;
class GUI;
class Server;

/**
 * @brief GUI command name -> parser member pointer (defined in gui_execute_cmds.cpp).
 * @details Table-driven dispatch (Command pattern) for GUI commands; GUI
 *          commands have no tick cost, so the value is just the parser method.
 */
extern std::map<std::string, void (GuiCommandParser::*)(std::shared_ptr<GUI>, const std::string &command, INetworkProvider &network)> gui_cmds;

#endif /* !GUI_EXECUTE_CMDS_HPP_ */
