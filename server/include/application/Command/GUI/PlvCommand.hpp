/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlvCommand.hpp
*/

#ifndef PLVCOMMAND_HPP_
#define PLVCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class PlvCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

    void setPlayerId(int id);
    int getPlayerId() const;

private:
    int _playerId = 0;
};

#endif /* !PLVCOMMAND_HPP_ */