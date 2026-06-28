/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SegEvent.hpp
*/

#ifndef SEGEVENT_HPP_
#define SEGEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include <string>

/**
 * @class SegEvent
 * @ingroup visitor_event
 * @brief Event: end of game, a team won.
 * @par Protocol (server -> GUI): "seg N\n" (N = winning team name).
 */
class SegEvent : public IEventElement {
public:
    /** @brief Builds the end-of-game event for the winning team. */
    SegEvent(const std::string& teamName);
    ~SegEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(SegEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    const std::string& getTeamName() const;
private:
    std::string _teamName;
};

#endif /* !SEGEVENT_HPP_ */
