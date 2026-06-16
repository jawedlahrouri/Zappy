/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForwardCommand.hpp
*/

#ifndef FORWARDCOMMAND_HPP_
#define FORWARDCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class ForwardCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !FORWARDCOMMAND_HPP_ */
