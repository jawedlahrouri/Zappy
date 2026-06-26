/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PfkEvent.hpp
*/

#ifndef PFKEVENT_HPP_
#define PFKEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class PfkEvent
 * @ingroup visitor_event
 * @brief Event: a player forked (laid an egg).
 * @par Protocol (server -> GUI): "pfk #n\n" (n = forking player id).
 */
class PfkEvent : public IEventElement {
public:
    /** @brief Builds the event for the forking player id. */
    PfkEvent(int playerId);
    ~PfkEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PfkEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;

private:
    int _playerId;
};

#endif /* !PFKEVENT_HPP_ */
