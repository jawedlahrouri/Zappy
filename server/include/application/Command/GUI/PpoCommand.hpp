/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.hpp
*/

#ifndef PPOCOMMAND_HPP_
#define PPOCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class PpoCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

    void setPlayerId(int id);
    int getPlayerId() const;

private:
    int _playerId = 0;
};

#endif /* !PPOCOMMAND_HPP_ */
