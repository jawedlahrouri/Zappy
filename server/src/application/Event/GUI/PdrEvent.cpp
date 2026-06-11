/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdrEvent.cpp
*/

#include "Event/GUI/PdrEvent.hpp"
#include "Event/GUI/EventVisitor.hpp"

#include "ServerException.hpp"
#include <string>

PdrEvent::PdrEvent(int playerId, int resource) : _playerId(playerId), _resource(resource) {}

void PdrEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PdrEvent*>(this));
}

int PdrEvent::getPlayerId() const {
    return _playerId;
}

int PdrEvent::getResource() const {
    return _resource;
}

void EventVisitor::visit(PdrEvent *event) {
    try {
        _network.sendToGuis("pdr " + std::to_string(event->getPlayerId()) + " " + std::to_string(event->getResource()));
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PdrEvent): ") + e.what());
    }
}
