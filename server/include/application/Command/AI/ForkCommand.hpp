/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForkCommand.hpp
*/

#ifndef FORKCOMMAND_HPP_
#define FORKCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class ForkCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !FORKCOMMAND_HPP_ */
