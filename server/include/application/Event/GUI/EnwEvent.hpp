/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EnwEvent.hpp
*/

#ifndef ENWEVENT_HPP_
#define ENWEVENT_HPP_

#include "IEventElement.hpp"

class EnwEvent : public IEventElement {
public:
    EnwEvent(int eggId, int playerId, int x, int y);
    ~EnwEvent() override = default;

    void Accept(IEventVisitor *visitor) const override;
    
    int getEggId() const;
    int getPlayerId() const;
    int getX() const;
    int getY() const;

private:
    int _eggId;
    int _playerId;
    int _x;
    int _y;
};

#endif /* !ENWEVENT_HPP_ */
