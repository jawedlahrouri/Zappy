/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BctEvent.hpp
*/

#ifndef BCTEVENT_HPP_
#define BCTEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

/**
 * @class BctEvent
 * @ingroup visitor_event
 * @brief Event: a tile's content changed.
 * @par Protocol (server -> GUI): "bct X Y q0 q1 q2 q3 q4 q5 q6\n".
 */
class BctEvent : public IEventElement {
public:
    /** @brief Builds the event from a tile position and its 7 resource counts. */
    BctEvent(int x, int y, int q0, int q1, int q2, int q3, int q4, int q5, int q6);
    ~BctEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(BctEvent*). */
    void Accept(IEventVisitor *visitor) const override;

    int getX() const;
    int getY() const;
    int getQ0() const;
    int getQ1() const;
    int getQ2() const;
    int getQ3() const;
    int getQ4() const;
    int getQ5() const;
    int getQ6() const;

private:
    int _x;
    int _y;
    int _q0;
    int _q1;
    int _q2;
    int _q3;
    int _q4;
    int _q5;
    int _q6;
};

#endif /* !BCTEVENT_HPP_ */
