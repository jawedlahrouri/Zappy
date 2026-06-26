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

/**
 * @class EjectCommand
 * @ingroup visitor_ai
 * @brief AI "Eject" command DTO (push players off the current tile).
 * @par Protocol: Request "Eject\n" -> Response "ok\n" or "ko\n". Duration: 7/f.
 */
class EjectCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(EjectCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !EJECTCOMMAND_HPP_ */
