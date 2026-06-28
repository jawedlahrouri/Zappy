/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinEvent.cpp
*/

#include "application/Event/GUI/PinEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PinEvent::PinEvent(int playerId, int x, int y, int q0, int q1, int q2, int q3, int q4, int q5, int q6)
    : _playerId(playerId), _x(x), _y(y), _q0(q0), _q1(q1), _q2(q2), _q3(q3), _q4(q4), _q5(q5), _q6(q6) {}

void PinEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PinEvent*>(this));
}

int PinEvent::getPlayerId() const { return _playerId; }
int PinEvent::getX() const { return _x; }
int PinEvent::getY() const { return _y; }
int PinEvent::getQ0() const { return _q0; }
int PinEvent::getQ1() const { return _q1; }
int PinEvent::getQ2() const { return _q2; }
int PinEvent::getQ3() const { return _q3; }
int PinEvent::getQ4() const { return _q4; }
int PinEvent::getQ5() const { return _q5; }
int PinEvent::getQ6() const { return _q6; }

void EventVisitor::visit(PinEvent *event) {
    try {
        std::string payload = "pin #" + std::to_string(event->getPlayerId()) + " " +
                            std::to_string(event->getX()) + " " +
                            std::to_string(event->getY()) + " " +
                            std::to_string(event->getQ0()) + " " +
                            std::to_string(event->getQ1()) + " " +
                            std::to_string(event->getQ2()) + " " +
                            std::to_string(event->getQ3()) + " " +
                            std::to_string(event->getQ4()) + " " +
                            std::to_string(event->getQ5()) + " " +
                            std::to_string(event->getQ6()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PinEvent): ") + e.what());
    }
}
