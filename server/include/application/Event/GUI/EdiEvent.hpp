/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EdiEvent.hpp
*/

#ifndef EDIEVENT_HPP_
#define EDIEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class EdiEvent
 * @ingroup visitor_event
 * @brief Event: an egg died (was destroyed).
 * @par Protocol (server -> GUI): "edi #e\n" (e = egg id).
 */
class EdiEvent : public IEventElement {
public:
    /** @brief Builds the event for the given egg id. */
    EdiEvent(int eggId);
    ~EdiEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(EdiEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getEggId() const;

private:
    int _eggId;
};

#endif /* !EDIEVENT_HPP_ */
