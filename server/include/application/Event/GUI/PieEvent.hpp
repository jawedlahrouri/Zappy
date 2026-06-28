/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PieEvent.hpp
*/

#ifndef PIEEVENT_HPP_
#define PIEEVENT_HPP_

#include "IEventElement.hpp"

/**
 * @class PieEvent
 * @ingroup visitor_event
 * @brief Event: an incantation ended on a tile.
 * @par Protocol (server -> GUI): "pie X Y R\n" (pos, R = 1 success / 0 failure).
 */
class PieEvent : public IEventElement {
public:
    /** @brief Builds the event from the tile and the result flag. */
    PieEvent(int x, int y, int result);
    ~PieEvent() override = default;

    /** @brief Dispatches to @c IEventVisitor::visit(PieEvent*). */
    void Accept(IEventVisitor *visitor) const override;
    
    int getX() const;
    int getY() const;
    int getResult() const;

private:
    int _x;
    int _y;
    int _result;
};

#endif /* !PIEEVENT_HPP_ */
