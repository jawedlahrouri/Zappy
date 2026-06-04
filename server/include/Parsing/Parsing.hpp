/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Parsing.hpp
*/

#ifndef PARSING_HPP_
#define PARSING_HPP_

#include <string>
#include <vector>
#include <memory>

class Server;
class AI;
class GUI;

class Parsing {
    public:
        Parsing() = default;
        ~Parsing() = default;

        static void parse_ai_command(std::shared_ptr<AI> client, const std::string &command);
        static void parse_gui_command(std::shared_ptr<GUI> client, const std::string &command, Server &server);

    private:
        static std::vector<std::string> split_command(const std::string &command);
};

#endif /* !PARSING_HPP_ */
