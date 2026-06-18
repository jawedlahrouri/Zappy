/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.hpp
*/

#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "application/Server/Server.hpp"
#include <memory>
#include "INetworkProvider.hpp"
#include "AI.hpp"
#include "GUI.hpp"


class Login {
    public:
        static bool login_ai(std::shared_ptr<AI> ai, const std::string &team_name, INetworkProvider &network);
        static void login_gui(std::shared_ptr<GUI> gui, INetworkProvider &network);
};

#endif /* !LOGIN_HPP_ */