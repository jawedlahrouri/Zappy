/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EboEvent.hpp
*/

#ifndef EBOEVENT_HPP_
#define EBOEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class EboEvent
 * @ingroup visitor_event
 * @brief Event: a player connected for (hatched from) an egg.
 * @par Protocol (server -> GUI): "ebo #e\n" (e = egg id).
 */
class EboEvent : public IEventElement {
public:
    /** @brief Builds the event for the given egg id. */
    EboEvent(int eggId);
    ~EboEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(EboEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getEggId() const;

private:
    int _eggId;
};

#endif /* !EBOEVENT_HPP_ */
