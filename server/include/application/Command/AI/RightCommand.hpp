/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.hpp
*/

#ifndef RIGHTCOMMAND_HPP_
#define RIGHTCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class RightCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !RIGHTCOMMAND_HPP_ */
