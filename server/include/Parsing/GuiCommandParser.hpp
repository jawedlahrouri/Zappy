/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GuiCommandParser.hpp
*/

#ifndef GUI_COMMAND_PARSER_HPP_
#define GUI_COMMAND_PARSER_HPP_

#include <string>
#include <memory>

class GUI;
class Server;

class GuiCommandParser {
    public:
        GuiCommandParser() = default;
        ~GuiCommandParser() = default;

        void parse_gui_msz(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_bct(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_mct(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_tna(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_ppo(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pic(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pie(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_plv(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pin(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pdr(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pgt(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pbc(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pfc(std::shared_ptr<GUI> client, const std::string &command, Server &server);
        void parse_gui_pfk(std::shared_ptr<GUI> client, const std::string &command, Server &server);
};

#endif /* !GUI_COMMAND_PARSER_HPP_ */
