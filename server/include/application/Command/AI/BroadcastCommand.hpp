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

class BroadcastCommand : public BaseAiCommand {
public:
  void Accept(IAiVisitor *visitor) override;
  void setMessage(const std::string& message);
  std::string getMessage() const;

private:
  std::string _message;
};

#endif /* !BROADCASTCOMMAND_HPP_ */
