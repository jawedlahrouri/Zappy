/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoEvent.cpp
*/

#include "application/Event/GUI/PpoEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PpoEvent::PpoEvent(int playerId, int x, int y, int orientation)
    : _playerId(playerId), _x(x), _y(y), _orientation(orientation) {}

void PpoEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PpoEvent*>(this));
}

int PpoEvent::getPlayerId() const { return _playerId; }
int PpoEvent::getX() const { return _x; }
int PpoEvent::getY() const { return _y; }
int PpoEvent::getOrientation() const { return _orientation; }

void EventVisitor::visit(PpoEvent *event) {
    try {
        std::string payload = "ppo #" + std::to_string(event->getPlayerId()) + " " +
                            std::to_string(event->getX()) + " " +
                            std::to_string(event->getY()) + " " +
                            std::to_string(event->getOrientation()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PpoEvent): ") + e.what());
    }
}
