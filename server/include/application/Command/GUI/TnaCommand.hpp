/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TnaCommand.hpp
*/

#ifndef TNACOMMAND_HPP_
#define TNACOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class TnaCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !TNACOMMAND_HPP_ */
