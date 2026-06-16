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

SegEvent::SegEvent(const std::string& teamName) : _teamName(teamName) {}

const std::string& SegEvent::getTeamName() const {
    return _teamName;
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
        std::string payload = "seg " + event->getTeamName() + "\n";
        _network.sendToGuis(payload);
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(SegEvent): ") + e.what());
    }
}