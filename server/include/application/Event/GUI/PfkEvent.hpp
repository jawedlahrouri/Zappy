/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PfkEvent.hpp
*/

#ifndef PFKEVENT_HPP_
#define PFKEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PfkEvent : public IEventElement {
public:
    PfkEvent(int playerId);
    ~PfkEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;

private:
    int _playerId;
};

#endif /* !PFKEVENT_HPP_ */
