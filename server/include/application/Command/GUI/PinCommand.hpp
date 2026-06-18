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

    void setPlayerId(int id);
    int getPlayerId() const;

private:
    int _playerId = 0;
};

#endif /* !PINCOMMAND_HPP_ */