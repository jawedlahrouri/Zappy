/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PexEvent.cpp
*/

#include "application/Event/GUI/PexEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include <string>

PexEvent::PexEvent(int playerId) : _playerId(playerId) {}

void PexEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PexEvent*>(this));
}

int PexEvent::getPlayerId() const {
    return _playerId;
}

void EventVisitor::visit(PexEvent *event) {
    try {
        std::string payload = "pex #" + std::to_string(event->getPlayerId()) + "\n";
        _network.sendToGuis(payload);
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PexEvent): ") + e.what());
    }
}
