/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PnwEvent.cpp
*/

#include "application/Event/GUI/PnwEvent.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "AI.hpp"
#include "ServerException.hpp"
#include <string>

/**
 * @brief Builds a "new player" event for the given AI.
 * @param ai The player that just connected/hatched.
 */
PnwEvent::PnwEvent(std::shared_ptr<AI> ai) : _ai(ai) {}

/**
 * @brief Double-dispatch hook: routes this event to @c visit(PnwEvent*).
 * @param visitor Event visitor whose @c PnwEvent overload serialises it.
 */
void PnwEvent::Accept(IEventVisitor *visitor) const {
    visitor->visit(const_cast<PnwEvent*>(this));
}

std::shared_ptr<AI> PnwEvent::getAi() const {
    return _ai;
}

/**
 * @brief Serialises the "new player" event and pushes it to the GUI(s).
 *
 * @par Protocol (GUI protocol)
 * Server -> GUI: "pnw #n X Y O L N\n"
 *   n = player id, X/Y = position, O = orientation, L = level, N = team name.
 *
 * @details Sent to a single GUI when @c _targetFd is set (e.g. answering a GUI
 * that just connected) or broadcast to every GUI otherwise. The trailing
 * newline is added by the send path.
 *
 * @param event DTO carrying the player that appeared.
 */
void EventVisitor::visit(PnwEvent *event) {
    try {
        auto ai = event->getAi();
        std::string payload = "pnw #" + std::to_string(ai->getId()) + " " +
                            std::to_string(ai->getX()) + " " + std::to_string(ai->getY()) + " " +
                            std::to_string(static_cast<int>(ai->getOrientation())) + " " + 
                            std::to_string(ai->getLevel()) + " " + ai->getTeamName();
        if (_targetFd != -1) {
            _network.sendResponse(_targetFd, payload);
        } else {
            _network.sendToGuis(payload);
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("EventVisitor::visit(PnwEvent): ") + e.what());
    }
}
