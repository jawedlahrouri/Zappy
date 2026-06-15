/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectNbrCommand.hpp
*/

#ifndef CONNECTNBRCOMMAND_HPP_
#define CONNECTNBRCOMMAND_HPP_

#include "application/Command/AI/IAiCommandElement.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"

class ConnectNbrCommand : public IAiCommandElement {
public:
  void Accept(IAiVisitor *visitor) override;
  void setClient(std::shared_ptr<AI> client) { _client = client; }
  std::shared_ptr<AI> getClient() const { return _client; }

private:
  std::shared_ptr<AI> _client;
};

#endif /* !CONNECTNBRCOMMAND_HPP_ */
