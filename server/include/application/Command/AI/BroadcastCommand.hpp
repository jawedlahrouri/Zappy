/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BroadcastCommand.hpp
*/

#ifndef BROADCASTCOMMAND_HPP_
#define BROADCASTCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"
#include <memory>

/**
 * @class BroadcastCommand
 * @ingroup visitor_ai
 * @brief AI "Broadcast" command DTO (sound message to all players).
 * @par Protocol: Request "Broadcast text\n" -> Response "ok\n"; others receive
 *      "message K, text\n" (K = incoming sound direction). Duration: 7/f.
 */
class BroadcastCommand : public BaseAiCommand {
public:
  /** @brief Dispatches to @c IAiVisitor::visit(BroadcastCommand*). */
  void Accept(IAiVisitor *visitor) override;
  /** @brief Sets the broadcast message text. */
  void setMessage(const std::string& message);
  /** @brief Returns the broadcast message text. */
  std::string getMessage() const;

private:
  std::string _message; /**< Broadcast message text. */
};

#endif /* !BROADCASTCOMMAND_HPP_ */
