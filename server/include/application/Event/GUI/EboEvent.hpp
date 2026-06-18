/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EboEvent.hpp
*/

#ifndef EBOEVENT_HPP_
#define EBOEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class EboEvent : public IEventElement {
public:
    EboEvent(int eggId);
    ~EboEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getEggId() const;

private:
    int _eggId;
};

#endif /* !EBOEVENT_HPP_ */
