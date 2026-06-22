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

class TakeCommand : public BaseAiCommand {
public:
  void Accept(IAiVisitor *visitor) override;
  void setResourceName(const std::string& name);
  std::string getResourceName() const;

private:
  std::string _resourceName;
};

#endif /* !TAKECOMMAND_HPP_ */
