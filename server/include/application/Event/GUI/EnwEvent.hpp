/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EnwEvent.hpp
*/

#ifndef ENWEVENT_HPP_
#define ENWEVENT_HPP_

#include "IEventElement.hpp"

/**
 * @class EnwEvent
 * @ingroup visitor_event
 * @brief Event: an egg was laid by a player.
 * @par Protocol (server -> GUI): "enw #e #n X Y\n" (e = egg, n = layer, X Y = pos).
 */
class EnwEvent : public IEventElement {
public:
    /** @brief Builds the event from egg id, laying player id and position. */
    EnwEvent(int eggId, int playerId, int x, int y);
    ~EnwEvent() override = default;

    /** @brief Dispatches to @c IEventVisitor::visit(EnwEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    
    int getEggId() const;
    int getPlayerId() const;
    int getX() const;
    int getY() const;

private:
    int _eggId;
    int _playerId;
    int _x;
    int _y;
};

#endif /* !ENWEVENT_HPP_ */
