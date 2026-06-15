/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SegEvent.cpp
*/

#include "application/Event/GUI/SegEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "AI.hpp"
#include "ServerException.hpp"
#include <string>

SegEvent::SegEvent(int teamId) : _teamId(teamId) {}

int SegEvent::getTeamId() const {
    return _teamId;
}

void SegEvent::Accept(IEventVisitor *visitor) const {
    try {
        visitor->visit(const_cast<SegEvent*>(this));
    } catch (const std::exception &e) {
        throw ServerException(std::string("SegEvent::Accept: ") + e.what());
    }
}

void EventVisitor::visit(SegEvent *event) {
    try {
        std::string payload = "seg " + std::to_string(event->getTeamId());
        _network.sendToGuis(payload);
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(SegEvent): ") + e.what());
    }
}