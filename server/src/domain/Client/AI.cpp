/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.cpp
*/

#include "Client/AI.hpp"
#include "Network/INetworkProvider.hpp"
#include "GameEngine/GameEngine.hpp"

AI::AI(std::unique_ptr<ISocket> clientSocket, int playerId)
    : Client(std::move(clientSocket), playerId), _level(1), _orientation(1), _foodTicks(126)
{
    setType(ClientType::AI);
    _inventory["food"] = 10;
}

void AI::update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove)
{
    engine.updateAi(std::dynamic_pointer_cast<AI>(self), network, clientsToRemove);
}
