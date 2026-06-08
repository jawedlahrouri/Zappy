/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.cpp
*/

#include "Server/Server.hpp"
#include "Server/login.hpp"
#include "Parsing/Respond.hpp"
#include "GameEngine/GameEngine.hpp"


#include <iostream>
#include <string>

// protocol:
// <-- WELCOME (sent on connection accept)
// --> TEAM-NAME
// <-- CLIENT-NUM (unused slots)
// <-- X Y (map dimensions)
void Login::login_ai(std::shared_ptr<AI> ai, const std::string &team_name, Server &server)
{
    int count = 0;
    for (const auto &[fd_client, client] : server.get_clients()) {
        if (client->type == ClientType::AI) {
            auto ai_client = std::dynamic_pointer_cast<AI>(client);
            if (ai_client && ai_client->teamName == team_name) {
                count++;
            }
        }
    }
    int slots = server.get_clients_nb() - count;

    int fd = ai->socket.get_socket();
    Respond::send_response(server, fd, std::to_string(slots));
    if (server.get_game_engine()) {
        Respond::send_response(server, fd, std::to_string(server.get_game_engine()->get_width()) + " " + std::to_string(server.get_game_engine()->get_height()));
    } else {
        Respond::send_response(server, fd, "0 0");
    }
}

// protocol:
// <-- WELCOME (sent on connection accept)
// --> GRAPHIC
// <-- msz X Y\n
// <-- sgt T\n
// <-- tna N\n * nbr_teams
void Login::login_gui(std::shared_ptr<GUI> gui, Server &server)
{
    int fd = gui->socket.get_socket();

    if (server.get_game_engine()) {
        Respond::send_response(server, fd, "msz " + std::to_string(server.get_game_engine()->get_width()) + " " + std::to_string(server.get_game_engine()->get_height()));
    } else {
        Respond::send_response(server, fd, "msz 0 0");
    }
    Respond::send_response(server, fd, "sgt " + std::to_string(server.get_freq()));
    for (const auto &team : server.get_team_names()) {
        Respond::send_response(server, fd, "tna " + team);
    }
}
