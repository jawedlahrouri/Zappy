/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.cpp
*/

#include "Client/AI.hpp"
#include "Server/Server.hpp"
#include "GameEngine/GameEngine.hpp"

AI::AI(Socket clientSocket, int player_id)
    : Client(std::move(clientSocket)), id(player_id), level(1), orientation(1), food_ticks(126)
{
    type = ClientType::AI;
}

void AI::update(std::shared_ptr<Client> self, Server& server, GameEngine& engine, std::vector<int>& clients_to_remove)
{
    engine.update_ai(std::dynamic_pointer_cast<AI>(self), server, clients_to_remove);
}
