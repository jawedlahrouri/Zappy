/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoEvent.hpp
*/

#ifndef PPOEVENT_HPP_
#define PPOEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PpoEvent : public IEventElement {
public:
    PpoEvent(int playerId, int x, int y, int orientation);
    ~PpoEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
    int getX() const;
    int getY() const;
    int getOrientation() const;

private:
    int _playerId;
    int _x;
    int _y;
    int _orientation;
};

#endif /* !PPOEVENT_HPP_ */
