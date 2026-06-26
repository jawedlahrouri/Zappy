/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SetCommand.hpp
*/

#ifndef SETCOMMAND_HPP_
#define SETCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

/**
 * @class SetCommand
 * @ingroup visitor_ai
 * @brief AI "Set" command DTO (drop an object on the tile).
 * @par Protocol: Request "Set object\n" -> Response "ok\n" or "ko\n". Duration: 7/f.
 */
class SetCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(SetCommand*). */
    void Accept(IAiVisitor *visitor) override;
  /** @brief Sets the resource name to drop. */
  void setResourceName(const std::string& name);
  /** @brief Returns the resource name to drop. */
  std::string getResourceName() const;
private:
    std::string _resourceName; /**< Resource to drop. */
};

#endif /* !SETCOMMAND_HPP_ */
