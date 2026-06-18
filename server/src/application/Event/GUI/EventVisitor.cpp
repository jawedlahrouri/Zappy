/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EventVisitor.cpp
*/

#include "application/Event/GUI/EventVisitor.hpp"

EventVisitor::EventVisitor(INetworkProvider &network) : _network(network)
{
}

void EventVisitor::setTargetClient(int fd)
{
    _targetFd = fd;
}
