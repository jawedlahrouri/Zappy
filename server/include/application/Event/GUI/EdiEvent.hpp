/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EdiEvent.hpp
*/

#ifndef EDIEVENT_HPP_
#define EDIEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class EdiEvent : public IEventElement {
public:
    EdiEvent(int eggId);
    ~EdiEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getEggId() const;

private:
    int _eggId;
};

#endif /* !EDIEVENT_HPP_ */
