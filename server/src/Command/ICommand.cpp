/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Command.cpp
*/

#include "Command/ICommand.hpp"
#include "Command/MapCommand.hpp"
#include "Command/PlayerCommand.hpp"
#include "Command/GameCommand.hpp"
#include "Server/Server.hpp"
#include "GameEngine/GameEngine.hpp"

Command::Command(Server &server)
    : _server(server)
{
}

//probablement temporaire ici je vais voir pour faire un truc plus propre
void Command::visit(MapCommand *element) const {
    int count = 0;
    std::string team_name = element->get_team_name();


    if (!element)
        return;
    if (_server.get_game_engine()) {
        element->set_map_width(_server.get_game_engine()->get_width());
        element->set_map_height(_server.get_game_engine()->get_height());
    } else {
        element->set_map_width(0);
        element->set_map_height(0);
    }
    for (const auto &[fd, client] : _server.get_clients()) {
        if (client->type == ClientType::AI) {
            auto ai = std::dynamic_pointer_cast<AI>(client);
            if (ai && ai->teamName == team_name) {
                count++;
            }
        }
    }
    element->set_connection_player(_server.get_clients_nb() - count);
}

void Command::visit(PlayerCommand *element) const {
    if (!element)
        return;
    
    element->set_time(_server.get_freq());
}

void Command::visit(GameCommand *element) const {
    (void)element;
}
