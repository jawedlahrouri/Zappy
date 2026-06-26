/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PgtEvent.hpp
*/

#ifndef PGTEVENT_HPP_
#define PGTEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class PgtEvent
 * @ingroup visitor_event
 * @brief Event: a player picked up a resource.
 * @par Protocol (server -> GUI): "pgt #n i\n" (n = player, i = resource index).
 */
class PgtEvent : public IEventElement {
public:
    /** @brief Builds the event from the player id and collected resource index. */
    PgtEvent(int playerId, int resourceId);
    ~PgtEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PgtEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
    int getResourceId() const;

private:
    int _playerId;
    int _resourceId;
};

#endif /* !PGTEVENT_HPP_ */
