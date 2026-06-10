/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdiEvent.hpp
*/

#ifndef PDIEVENT_HPP_
#define PDIEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PdiEvent : public IEventElement {
public:
    PdiEvent(int playerId);
    ~PdiEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;
    int getPlayerId() const;
private:
    int _playerId;
};

#endif /* !PDIEVENT_HPP_ */
