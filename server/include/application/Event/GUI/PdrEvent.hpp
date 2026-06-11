/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PdrEvent.hpp
*/

#ifndef PDREVENT_HPP_
#define PDREVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class PdrEvent : public IEventElement {
public:
    PdrEvent(int playerId, int resource);
    ~PdrEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;
    int getPlayerId() const;
    int getResource() const;
private:
    int _playerId;
    int _resource;
};

#endif /* !PDREVENT_HPP_ */
