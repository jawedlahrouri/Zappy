/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PgtEvent.cpp
*/

#include "application/Event/GUI/PgtEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PgtEvent::PgtEvent(int playerId, int resourceId)
    : _playerId(playerId), _resourceId(resourceId) {}

void PgtEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PgtEvent*>(this));
}

int PgtEvent::getPlayerId() const { return _playerId; }
int PgtEvent::getResourceId() const { return _resourceId; }

void EventVisitor::visit(PgtEvent *event) {
    try {
        std::string payload = "pgt #" + std::to_string(event->getPlayerId()) + " " +
                            std::to_string(event->getResourceId()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PgtEvent): ") + e.what());
    }
}
