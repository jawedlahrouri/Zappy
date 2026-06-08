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
#include "Command/ICommand.hpp"
#include "Command/MapCommand.hpp"
#include "Command/PlayerCommand.hpp"
#include <iostream>
#include <string>

// protocol:
// <-- WELCOME (sent on connection accept)
// --> TEAM-NAME
// <-- CLIENT-NUM (unused slots)
// <-- X Y (map dimensions)
void Login::login_ai(std::shared_ptr<AI> ai, const std::string &team_name, Server &server)
{
    Command visitor(server);
    MapCommand mapCmd;

    mapCmd.set_team_name(team_name);    
    mapCmd.Accept(&visitor);
    int fd = ai->socket.get_socket();
    Respond::send_response(server, fd, std::to_string(mapCmd.get_connection_player()));
    Respond::send_response(server, fd, std::to_string(mapCmd.get_map_width()) + " " + std::to_string(mapCmd.get_map_height()));
}

// protocol:
// <-- WELCOME (sent on connection accept)
// --> GRAPHIC
// <-- msz X Y\n
// <-- sgt T\n
// <-- tna N\n * nbr_teams
void Login::login_gui(std::shared_ptr<GUI> gui, Server &server)
{
    Command visitor(server);
    MapCommand mapCmd;
    mapCmd.Accept(&visitor);

    PlayerCommand playerCmd;
    playerCmd.Accept(&visitor);

    int fd = gui->socket.get_socket();

    Respond::send_response(server, fd, "msz " + std::to_string(mapCmd.get_map_width()) + " " + std::to_string(mapCmd.get_map_height()));
    Respond::send_response(server, fd, "sgt " + std::to_string(playerCmd.get_time()));
    for (const auto &team : server.get_team_names()) {
        Respond::send_response(server, fd, "tna " + team);
    }
}
