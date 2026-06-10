/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.cpp
*/

#include "Client/AI.hpp"
#include "Network/INetworkProvider.hpp"
#include "GameEngine/GameEngine.hpp"

AI::AI(Socket clientSocket, int playerId)
    : Client(std::move(clientSocket)), m_id(playerId), m_level(1), m_orientation(1), m_foodTicks(126)
{
    setType(ClientType::AI);
}

void AI::update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove)
{
    engine.updateAi(std::dynamic_pointer_cast<AI>(self), network, clientsToRemove);
}
