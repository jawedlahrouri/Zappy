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

class SetCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;
  void setResourceName(const std::string& name);
  std::string getResourceName() const;
private:
    std::string _resourceName;
};

#endif /* !SETCOMMAND_HPP_ */
