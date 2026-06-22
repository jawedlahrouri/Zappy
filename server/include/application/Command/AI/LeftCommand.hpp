/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LeftCommand.hpp
*/

#ifndef LEFTCOMMAND_HPP_
#define LEFTCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class LeftCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !LEFTCOMMAND_HPP_ */
