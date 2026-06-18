/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PgtEvent.hpp
*/

#ifndef PGTEVENT_HPP_
#define PGTEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PgtEvent : public IEventElement {
public:
    PgtEvent(int playerId, int resourceId);
    ~PgtEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
    int getResourceId() const;

private:
    int _playerId;
    int _resourceId;
};

#endif /* !PGTEVENT_HPP_ */
