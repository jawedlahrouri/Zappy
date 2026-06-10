/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PnwEvent.cpp
*/

#include "Event/GUI/PnwEvent.hpp"
#include "Event/GUI/EventVisitor.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
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
        std::string payload = "pnw " + std::to_string(ai->id) + " " + 
                            std::to_string(ai->x) + " " + std::to_string(ai->y) + " " +
                            std::to_string(static_cast<int>(ai->orientation)) + " " + 
                            std::to_string(ai->level) + " " + ai->teamName;
        Respond::send_to_guis(_server, payload);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("EventVisitor::visit(PnwEvent): ") + e.what());
    }
}
