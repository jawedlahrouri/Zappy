/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BctCommand.hpp
*/

#ifndef BCTCOMMAND_HPP_
#define BCTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class BctCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !BCTCOMMAND_HPP_ */
