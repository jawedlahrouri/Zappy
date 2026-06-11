/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.hpp
*/

#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "Server/Server.hpp"
#include <memory>
#include "Network/INetworkProvider.hpp"
#include "Client/AI.hpp"
#include "Client/GUI.hpp"


class Login {
    public:
        static void login_ai(std::shared_ptr<AI> ai, const std::string &team_name, INetworkProvider &network);
        static void login_gui(std::shared_ptr<GUI> gui, INetworkProvider &network);
};

#endif /* !LOGIN_HPP_ */