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

class ConnectNbrCommand : public BaseAiCommand {
public:
  void Accept(IAiVisitor *visitor) override;

};

#endif /* !CONNECTNBRCOMMAND_HPP_ */
