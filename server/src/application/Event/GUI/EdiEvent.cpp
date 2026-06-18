/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EdiEvent.cpp
*/

#include "application/Event/GUI/EdiEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

EdiEvent::EdiEvent(int eggId) : _eggId(eggId) {}

void EdiEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<EdiEvent*>(this));
}

int EdiEvent::getEggId() const { return _eggId; }

void EventVisitor::visit(EdiEvent *event) {
    try {
        std::string payload = "edi " + std::to_string(event->getEggId()) + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(EdiEvent): ") + e.what());
    }
}
