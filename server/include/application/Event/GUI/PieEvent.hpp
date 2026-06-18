/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PieEvent.hpp
*/

#ifndef PIEEVENT_HPP_
#define PIEEVENT_HPP_

#include "IEventElement.hpp"

class PieEvent : public IEventElement {
public:
    PieEvent(int x, int y, int result);
    ~PieEvent() override = default;

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
