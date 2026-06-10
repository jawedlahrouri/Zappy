/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PnwEvent.hpp
*/

#ifndef PNWEVENT_HPP_
#define PNWEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include "Server/Server.hpp"
#include <memory>

class PnwEvent : public IEventElement {
public:
    PnwEvent(std::shared_ptr<AI> ai);
    ~PnwEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;
    std::shared_ptr<AI> getAi() const;
private:
    std::shared_ptr<AI> _ai;
};

#endif /* !PNWEVENT_HPP_ */
