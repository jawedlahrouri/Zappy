/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.hpp
*/

#ifndef MCTCOMMAND_HPP_
#define MCTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class MctCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !MCTCOMMAND_HPP_ */
