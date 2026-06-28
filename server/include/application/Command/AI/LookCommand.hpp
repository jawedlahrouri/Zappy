/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LookCommand.hpp
*/

#ifndef LOOKCOMMAND_HPP_
#define LOOKCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

/**
 * @class LookCommand
 * @ingroup visitor_ai
 * @brief AI "Look" command DTO (see surrounding tiles).
 * @par Protocol: Request "Look\n" -> Response "[tile0, tile1, ...]\n". Duration: 7/f.
 */
class LookCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(LookCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !LOOKCOMMAND_HPP_ */
