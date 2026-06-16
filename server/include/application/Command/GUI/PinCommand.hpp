/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinCommand.hpp
*/

#ifndef PINCOMMAND_HPP_
#define PINCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class PinCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

    void setTargetPlayerId(int id) { _targetPlayerId = id; }
    int getTargetPlayerId() const { return _targetPlayerId; }

private:
    int _targetPlayerId = 0;
};

#endif /* !PINCOMMAND_HPP_ */