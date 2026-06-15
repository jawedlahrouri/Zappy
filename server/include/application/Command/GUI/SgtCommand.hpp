/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SgtCommand.hpp
*/

#ifndef SGTCOMMAND_HPP_
#define SGTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class SgtCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !SGTCOMMAND_HPP_ */
