/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.cpp
*/

#include "AI.hpp"
#include "INetworkProvider.hpp"
#include "GameEngine.hpp"

AI::AI(std::unique_ptr<ISocket> clientSocket, int playerId)
    : Client(std::move(clientSocket), playerId), _level(1), _orientation(1), _foodTicks(126)
{
    setType(ClientType::AI);
    _inventory["food"] = 10;
    _inventory["linemate"] = 0;
    _inventory["deraumere"] = 0;
    _inventory["sibur"] = 0;
    _inventory["mendiane"] = 0;
    _inventory["phiras"] = 0;
    _inventory["thystame"] = 0;
}

void AI::update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove)
{
    engine.updateAi(std::dynamic_pointer_cast<AI>(self), network, clientsToRemove);
}
