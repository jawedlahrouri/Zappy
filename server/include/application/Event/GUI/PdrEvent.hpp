/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdrEvent.hpp
*/

#ifndef PDREVENT_HPP_
#define PDREVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class PdrEvent
 * @ingroup visitor_event
 * @brief Event: a player dropped a resource.
 * @par Protocol (server -> GUI): "pdr #n i\n" (n = player, i = resource index).
 */
class PdrEvent : public IEventElement {
public:
    /** @brief Builds the event from the player id and dropped resource index. */
    PdrEvent(int playerId, int resource);
    ~PdrEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PdrEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    int getPlayerId() const;
    int getResource() const;
private:
    int _playerId;
    int _resource;
};

#endif /* !PDREVENT_HPP_ */
