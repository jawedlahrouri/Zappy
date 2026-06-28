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

/**
 * @class LeftCommand
 * @ingroup visitor_ai
 * @brief AI "Left" command DTO (turn 90° counter-clockwise).
 * @par Protocol: Request "Left\n" -> Response "ok\n". Duration: 7/f.
 */
class LeftCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(LeftCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !LEFTCOMMAND_HPP_ */
