/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PieEvent.cpp
*/

#include "application/Event/GUI/PieEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include <string>

PieEvent::PieEvent(int x, int y, int result) : _x(x), _y(y), _result(result) {}

void PieEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PieEvent*>(this));
}

int PieEvent::getX() const {
    return _x;
}

int PieEvent::getY() const {
    return _y;
}

int PieEvent::getResult() const {
    return _result;
}

void EventVisitor::visit(PieEvent *event) {
    try {
        std::string payload = "pie " + std::to_string(event->getX()) + " " +
                            std::to_string(event->getY()) + " " +
                            std::to_string(event->getResult()) + "\n";
        _network.sendToGuis(payload);
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PieEvent): ") + e.what());
    }
}
