/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IncantationCommand.hpp
*/

#ifndef INCANTATIONCOMMAND_HPP_
#define INCANTATIONCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class IncantationCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !INCANTATIONCOMMAND_HPP_ */
