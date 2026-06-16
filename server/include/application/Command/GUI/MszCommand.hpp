/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MszCommand.hpp
*/

#ifndef MSZCOMMAND_HPP_
#define MSZCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class MszCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !MSZCOMMAND_HPP_ */
