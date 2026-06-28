/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.hpp
*/

/**
 * @file login.hpp
 * @brief Handshake helpers turning a pending client into a logged-in AI/GUI.
 */

#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "application/Server/Server.hpp"
#include <memory>
#include "INetworkProvider.hpp"
#include "AI.hpp"
#include "GUI.hpp"


/**
 * @class Login
 * @brief Static handlers implementing the Zappy connection handshake.
 */
class Login {
    public:
        /**
         * @brief Logs an AI into a team, assigning it an egg slot and replying.
         * @param ai The AI client to log in.
         * @param team_name Requested team name.
         * @param network Network/game facade.
         * @return @c true on success; @c false if the team is unknown or full.
         * @par Protocol (AI protocol)
         * On success the server replies "<remaining-slots>\n" then "<width> <height>\n".
         */
        static bool login_ai(std::shared_ptr<AI> ai, const std::string &team_name, INetworkProvider &network);

        /**
         * @brief Logs a GUI in and sends it the initial world snapshot.
         * @param gui The GUI client to log in.
         * @param network Network/game facade.
         */
        static void login_gui(std::shared_ptr<GUI> gui, INetworkProvider &network);
};

#endif /* !LOGIN_HPP_ */