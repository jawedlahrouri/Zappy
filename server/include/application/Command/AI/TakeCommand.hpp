/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TakeCommand.hpp
*/

#ifndef TAKECOMMAND_HPP_
#define TAKECOMMAND_HPP_

#include "application/Command/AI/IAiCommandElement.hpp"
#include "application/Command/AI/IAiVisitor.hpp"
#include "domain/Client/AI.hpp"

class TakeCommand : public IAiCommandElement {
public:
  void Accept(IAiVisitor *visitor) override;
  void setClient(std::shared_ptr<AI> client) { _client = client; }
  std::shared_ptr<AI> getClient() const { return _client; }
  void setResourceName(const std::string& name) { _resourceName = name; }
  std::string getResourceName() const { return _resourceName; }

private:
  std::shared_ptr<AI> _client;
  std::string _resourceName;
};

#endif /* !TAKECOMMAND_HPP_ */
