/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.hpp
*/

#ifndef RIGHTCOMMAND_HPP_
#define RIGHTCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

/**
 * @class RightCommand
 * @ingroup visitor_ai
 * @brief AI "Right" command DTO (turn 90° clockwise).
 * @par Protocol: Request "Right\n" -> Response "ok\n". Duration: 7/f.
 */
class RightCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(RightCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !RIGHTCOMMAND_HPP_ */
