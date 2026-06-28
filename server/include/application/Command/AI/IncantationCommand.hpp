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

/**
 * @class IncantationCommand
 * @ingroup visitor_ai
 * @brief AI "Incantation" command DTO (level-up ritual).
 * @par Protocol: Request "Incantation\n" -> "Elevation underway\n" then
 *      "Current level: k\n" (or "ko\n"). Duration: 300/f.
 */
class IncantationCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(IncantationCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !INCANTATIONCOMMAND_HPP_ */
