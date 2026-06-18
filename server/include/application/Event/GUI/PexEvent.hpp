/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PexEvent.hpp
*/

#ifndef PEXEVENT_HPP_
#define PEXEVENT_HPP_

#include "IEventElement.hpp"

class PexEvent : public IEventElement {
public:
    explicit PexEvent(int playerId);
    ~PexEvent() override = default;

    void Accept(IEventVisitor *visitor) const override;
    
    int getPlayerId() const;

private:
    int _playerId;
};

#endif /* !PEXEVENT_HPP_ */
