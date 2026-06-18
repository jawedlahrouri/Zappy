/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PfkEvent.cpp
*/

#include "application/Event/GUI/PfkEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PfkEvent::PfkEvent(int playerId) : _playerId(playerId) {}

void PfkEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PfkEvent*>(this));
}

int PfkEvent::getPlayerId() const { return _playerId; }

void EventVisitor::visit(PfkEvent *event) {
    try {
        std::string payload = "pfk " + std::to_string(event->getPlayerId()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PfkEvent): ") + e.what());
    }
}
