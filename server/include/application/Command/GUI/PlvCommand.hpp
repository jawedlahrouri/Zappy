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

};

#endif /* !PLVCOMMAND_HPP_ */
