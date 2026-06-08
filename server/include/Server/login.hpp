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

class Login {
    public:
        static void login_ai(std::shared_ptr<AI> ai, const std::string &team_name, Server &server);
        static void login_gui(std::shared_ptr<GUI> gui, Server &server);
};

#endif /* !LOGIN_HPP_ */