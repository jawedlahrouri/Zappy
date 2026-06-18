/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EnwEvent.cpp
*/

#include "application/Event/GUI/EnwEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include <string>

EnwEvent::EnwEvent(int eggId, int playerId, int x, int y)
    : _eggId(eggId), _playerId(playerId), _x(x), _y(y) {}

void EnwEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<EnwEvent*>(this));
}

int EnwEvent::getEggId() const {
    return _eggId;
}

int EnwEvent::getPlayerId() const {
    return _playerId;
}

int EnwEvent::getX() const {
    return _x;
}

int EnwEvent::getY() const {
    return _y;
}

void EventVisitor::visit(EnwEvent *event) {
    try {
        std::string payload = "enw " + std::to_string(event->getEggId()) + " " +
                            std::to_string(event->getPlayerId()) + " " +
                            std::to_string(event->getX()) + " " +
                            std::to_string(event->getY()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(EnwEvent): ") + e.what());
    }
}
