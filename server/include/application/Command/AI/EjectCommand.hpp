/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EjectCommand.hpp
*/

#ifndef EJECTCOMMAND_HPP_
#define EJECTCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class EjectCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !EJECTCOMMAND_HPP_ */
