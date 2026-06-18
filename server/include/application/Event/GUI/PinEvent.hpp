/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinEvent.hpp
*/

#ifndef PINEVENT_HPP_
#define PINEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PinEvent : public IEventElement {
public:
    PinEvent(int playerId, int x, int y, int q0, int q1, int q2, int q3, int q4, int q5, int q6);
    ~PinEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
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
    int _playerId;
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

#endif /* !PINEVENT_HPP_ */
