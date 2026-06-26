/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PexEvent.hpp
*/

#ifndef PEXEVENT_HPP_
#define PEXEVENT_HPP_

#include "IEventElement.hpp"

/**
 * @class PexEvent
 * @ingroup visitor_event
 * @brief Event: a player was ejected from its tile.
 * @par Protocol (server -> GUI): "pex #n\n" (n = ejected player id).
 */
class PexEvent : public IEventElement {
public:
    /** @brief Builds the event for the ejected player id. */
    explicit PexEvent(int playerId);
    ~PexEvent() override = default;

    /** @brief Dispatches to @c IEventVisitor::visit(PexEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    
    int getPlayerId() const;

private:
    int _playerId;
};

#endif /* !PEXEVENT_HPP_ */
