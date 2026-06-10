/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdiEvent.cpp
*/

#include "Event/GUI/EventVisitor.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include "Event/GUI/PdiEvent.hpp"
#include <string>

PdiEvent::PdiEvent(int playerId) : _playerId(playerId) {}

void PdiEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PdiEvent*>(this));
}

int PdiEvent::getPlayerId() const {
    return _playerId;
}

void EventVisitor::visit(PdiEvent *event) {
    try {
        Respond::send_to_guis(_server, "pdi " + std::to_string(event->getPlayerId()));
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("EventVisitor::visit(PdiEvent): ") + e.what());
    }
}
