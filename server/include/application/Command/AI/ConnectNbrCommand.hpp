/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectNbrCommand.hpp
*/

#ifndef CONNECTNBRCOMMAND_HPP_
#define CONNECTNBRCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"

/**
 * @class ConnectNbrCommand
 * @ingroup visitor_ai
 * @brief AI "Connect_nbr" command DTO (free team slots).
 * @par Protocol: Request "Connect_nbr\n" -> Response "value\n". Duration: 0.
 */
class ConnectNbrCommand : public BaseAiCommand {
public:
  /** @brief Dispatches to @c IAiVisitor::visit(ConnectNbrCommand*). */
  void Accept(IAiVisitor *visitor) override;

};

#endif /* !CONNECTNBRCOMMAND_HPP_ */
