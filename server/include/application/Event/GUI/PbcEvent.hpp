/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PbcEvent.hpp
*/

#ifndef PBCEVENT_HPP_
#define PBCEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include <string>

/**
 * @class PbcEvent
 * @ingroup visitor_event
 * @brief Event: a player broadcast a message.
 * @par Protocol (server -> GUI): "pbc #n M\n" (n = player, M = message).
 */
class PbcEvent : public IEventElement {
public:
    /** @brief Builds the event from the broadcasting player id and its message. */
    PbcEvent(int playerId, const std::string &message);
    ~PbcEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PbcEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
    std::string getMessage() const;

private:
    int _playerId;
    std::string _message;
};

#endif /* !PBCEVENT_HPP_ */
