/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TakeCommand.hpp
*/

#ifndef TAKECOMMAND_HPP_
#define TAKECOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"

/**
 * @class TakeCommand
 * @ingroup visitor_ai
 * @brief AI "Take" command DTO (pick up an object from the tile).
 * @par Protocol: Request "Take object\n" -> Response "ok\n" or "ko\n". Duration: 7/f.
 */
class TakeCommand : public BaseAiCommand {
public:
  /** @brief Dispatches to @c IAiVisitor::visit(TakeCommand*). */
  void Accept(IAiVisitor *visitor) override;
  /** @brief Sets the resource name to take. */
  void setResourceName(const std::string& name);
  /** @brief Returns the resource name to take. */
  std::string getResourceName() const;

private:
  std::string _resourceName; /**< Resource to pick up. */
};

#endif /* !TAKECOMMAND_HPP_ */
