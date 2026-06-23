/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PnwEvent.cpp
*/

#include "application/Event/GUI/PnwEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "AI.hpp"
#include "ServerException.hpp"
#include <string>

PnwEvent::PnwEvent(std::shared_ptr<AI> ai) : _ai(ai) {}

void PnwEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PnwEvent*>(this));
}

std::shared_ptr<AI> PnwEvent::getAi() const {
    return _ai;
}

void EventVisitor::visit(PnwEvent *event) {
    try {
        auto ai = event->getAi();
        std::string payload = "pnw #" + std::to_string(ai->getId()) + " " +
                            std::to_string(ai->getX()) + " " + std::to_string(ai->getY()) + " " +
                            std::to_string(static_cast<int>(ai->getOrientation())) + " " + 
                            std::to_string(ai->getLevel()) + " " + ai->getTeamName();
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PnwEvent): ") + e.what());
    }
}
