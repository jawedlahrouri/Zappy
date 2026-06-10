#include "Network/INetworkProvider.hpp"
/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AiCommandParser.hpp
*/

#ifndef AI_COMMAND_PARSER_HPP_
#define AI_COMMAND_PARSER_HPP_

#include <string>
#include <memory>
#include "Server/Server.hpp"

class AI;

class AiCommandParser {
    public:
        AiCommandParser() = default;
        ~AiCommandParser() = default;

        void parse_forward(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_right(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_left(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_look(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_inventory(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_broadcast(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_connect_nbr(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_fork(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_eject(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_take(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_set(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        void parse_incantation(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
};

#endif /* !AI_COMMAND_PARSER_HPP_ */
