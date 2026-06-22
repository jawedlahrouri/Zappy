/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InventoryCommand.hpp
*/

#ifndef INVENTORYCOMMAND_HPP_
#define INVENTORYCOMMAND_HPP_

#include "application/Command/AI/BaseAiCommand.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class InventoryCommand : public BaseAiCommand {
public:
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !INVENTORYCOMMAND_HPP_ */
