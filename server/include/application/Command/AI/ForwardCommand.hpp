/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForwardCommand.hpp
*/

#ifndef FORWARDCOMMAND_HPP_
#define FORWARDCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

/**
 * @class ForwardCommand
 * @ingroup visitor_ai
 * @brief AI "Forward" command DTO (move one tile ahead).
 * @par Protocol: Request "Forward\n" -> Response "ok\n". Duration: 7/f.
 */
class ForwardCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(ForwardCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !FORWARDCOMMAND_HPP_ */
