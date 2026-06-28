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
#include "application/Server/Server.hpp"
#include "INetworkProvider.hpp"
#include "AI.hpp"

/**
 * @class AiCommandParser
 * @ingroup visitor_ai
 * @brief Parses AI command lines and builds the matching command DTO/visitor.
 *
 * Each @c parse_* method is the parser half of one command's vertical slice: it
 * extracts the arguments, fills the DTO and dispatches it to an @ref AiVisitor.
 * Protocol details for each command live on the corresponding DTO class.
 */
class AiCommandParser {
    public:
        AiCommandParser() = default;
        ~AiCommandParser() = default;

        /**
         * @brief Routes a raw AI command line to its @c parse_* handler.
         * @param client Issuing AI player.
         * @param command Raw command line.
         * @param network Network/game facade.
         */
        void executeCommand(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);

        /** @brief Parses "Forward" (move one tile ahead). */

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
