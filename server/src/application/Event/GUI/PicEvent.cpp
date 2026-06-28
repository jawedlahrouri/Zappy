/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PicEvent.cpp
*/

#include "application/Event/GUI/PicEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "ServerException.hpp"

PicEvent::PicEvent(int x, int y, int level, const std::vector<int> &playerIds)
    : _x(x), _y(y), _level(level), _playerIds(playerIds) {}

void PicEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PicEvent*>(this));
}

int PicEvent::getX() const { return _x; }
int PicEvent::getY() const { return _y; }
int PicEvent::getLevel() const { return _level; }
const std::vector<int>& PicEvent::getPlayerIds() const { return _playerIds; }

void EventVisitor::visit(PicEvent *event) {
    try {
        std::string payload = "pic " + std::to_string(event->getX()) + " " +
                            std::to_string(event->getY()) + " " +
                            std::to_string(event->getLevel());
        for (int id : event->getPlayerIds()) {
            payload += " #" + std::to_string(id);
        }
        payload += "\n";

        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PicEvent): ") + e.what());
    }
}
