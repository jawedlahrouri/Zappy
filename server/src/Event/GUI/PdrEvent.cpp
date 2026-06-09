/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdrEvent.cpp
*/

#include "Event/GUI/PdrEvent.hpp"
#include "Event/GUI/EventVisitor.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <string>

PdrEvent::PdrEvent(int playerId, int resource) : _playerId(playerId), _resource(resource) {}

void PdrEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PdrEvent*>(this));
}

int PdrEvent::getPlayerId() const {
    return _playerId;
}

int PdrEvent::getResource() const {
    return _resource;
}

void EventVisitor::visit(PdrEvent *event) {
    try {
        Respond::send_to_guis(_server, "pdr " + std::to_string(event->getPlayerId()) + " " + std::to_string(event->getResource()));
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("EventVisitor::visit(PdrEvent): ") + e.what());
    }
}
