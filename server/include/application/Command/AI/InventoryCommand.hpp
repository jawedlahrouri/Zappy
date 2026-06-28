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

/**
 * @class InventoryCommand
 * @ingroup visitor_ai
 * @brief AI "Inventory" command DTO (list held resources).
 * @par Protocol: Request "Inventory\n" -> Response "[food n, ...]\n". Duration: 1/f.
 */
class InventoryCommand : public BaseAiCommand {
public:
    /** @brief Dispatches to @c IAiVisitor::visit(InventoryCommand*). */
    void Accept(IAiVisitor *visitor) override;

};

#endif /* !INVENTORYCOMMAND_HPP_ */
