/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdiEvent.cpp
*/

#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"
#include "application/Event/GUI/PdiEvent.hpp"

PdiEvent::PdiEvent(int playerId) : _playerId(playerId) {}

void PdiEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PdiEvent*>(this));
}

int PdiEvent::getPlayerId() const {
    return _playerId;
}

void EventVisitor::visit(PdiEvent *event) {
    try {
        _network.sendToGuis("pdi #" + std::to_string(event->getPlayerId()) + "\n");
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PdiEvent): ") + e.what());
    }
}
