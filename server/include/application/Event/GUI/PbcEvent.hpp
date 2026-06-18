/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PbcEvent.hpp
*/

#ifndef PBCEVENT_HPP_
#define PBCEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include <string>

class PbcEvent : public IEventElement {
public:
    PbcEvent(int playerId, const std::string &message);
    ~PbcEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getPlayerId() const;
    std::string getMessage() const;

private:
    int _playerId;
    std::string _message;
};

#endif /* !PBCEVENT_HPP_ */
