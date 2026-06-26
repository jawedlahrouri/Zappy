/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdiEvent.hpp
*/

#ifndef PDIEVENT_HPP_
#define PDIEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class PdiEvent
 * @ingroup visitor_event
 * @brief Event: a player died (or disconnected).
 * @par Protocol (server -> GUI): "pdi #n\n" (n = player id).
 */
class PdiEvent : public IEventElement {
public:
    /** @brief Builds the event for the given player id. */
    PdiEvent(int playerId);
    ~PdiEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PdiEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    int getPlayerId() const;
private:
    int _playerId;
};

#endif /* !PDIEVENT_HPP_ */
