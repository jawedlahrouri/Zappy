/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LookCommand.hpp
*/

#ifndef LOOKCOMMAND_HPP_
#define LOOKCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class LookCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !LOOKCOMMAND_HPP_ */
