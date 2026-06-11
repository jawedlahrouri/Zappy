/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BroadcastCommand.hpp
*/

#ifndef BROADCASTCOMMAND_HPP_
#define BROADCASTCOMMAND_HPP_

#include "Command/AI/IAiCommandElement.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"
#include <memory>

class BroadcastCommand : public IAiCommandElement {
public:
  void Accept(IAiVisitor *visitor) override;
  void setClient(std::shared_ptr<AI> client) { _client = client; }
  std::shared_ptr<AI> getClient() const { return _client; }

private:
  std::shared_ptr<AI> _client;
};

#endif /* !BROADCASTCOMMAND_HPP_ */
