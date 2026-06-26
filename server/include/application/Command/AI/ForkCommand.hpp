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

/**
 * @class ForkCommand
 * @ingroup visitor_ai
 * @brief AI "Fork" command DTO (lay an egg for a new slot).
 * @par Protocol: Request "Fork\n" -> Response "ok\n". Duration: 42/f.
 */
class ForkCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(ForkCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !FORKCOMMAND_HPP_ */
