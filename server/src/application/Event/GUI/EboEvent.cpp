/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EboEvent.cpp
*/

#include "application/Event/GUI/EboEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

EboEvent::EboEvent(int eggId) : _eggId(eggId) {}

void EboEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<EboEvent*>(this));
}

int EboEvent::getEggId() const { return _eggId; }

void EventVisitor::visit(EboEvent *event) {
    try {
        std::string payload = "ebo #" + std::to_string(event->getEggId()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(EboEvent): ") + e.what());
    }
}
