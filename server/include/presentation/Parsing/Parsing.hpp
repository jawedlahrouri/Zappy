/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Parsing.hpp
*/

#ifndef PARSING_HPP_
#define PARSING_HPP_
#include "INetworkProvider.hpp"

#include <string>
#include <vector>
#include <memory>

class Server;
class AI;
class GUI;

/**
 * @class Parsing
 * @brief Front door that routes a raw line to the AI or GUI command dispatch.
 */
class Parsing {
    public:
        Parsing() = default;
        ~Parsing() = default;

        /**
         * @brief Looks up and runs the AI command named on @p command.
         * @param client Issuing AI player.
         * @param command Raw command line.
         * @param network Network/game facade.
         */
        static void parse_ai_command(std::shared_ptr<AI> client, const std::string &command, INetworkProvider &network);
        /**
         * @brief Looks up and runs the GUI command named on @p command.
         * @param client Issuing GUI client.
         * @param command Raw command line.
         * @param network Network/game facade.
         */
        static void parse_gui_command(std::shared_ptr<GUI> client, const std::string &command, INetworkProvider &network);

    private:
        /** @brief Splits a command line into whitespace-separated tokens. */
        static std::vector<std::string> split_command(const std::string &command);
};

#endif /* !PARSING_HPP_ */
