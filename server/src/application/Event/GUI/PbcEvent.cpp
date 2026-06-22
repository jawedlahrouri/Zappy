/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PbcEvent.cpp
*/

#include "application/Event/GUI/PbcEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PbcEvent::PbcEvent(int playerId, const std::string &message)
    : _playerId(playerId), _message(message) {}

void PbcEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PbcEvent*>(this));
}

int PbcEvent::getPlayerId() const { return _playerId; }
std::string PbcEvent::getMessage() const { return _message; }

void EventVisitor::visit(PbcEvent *event) {
    try {
        std::string payload = "pbc " + std::to_string(event->getPlayerId()) + " " + event->getMessage() + "\n";
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PbcEvent): ") + e.what());
    }
}
