/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InventoryCommand.hpp
*/

#ifndef INVENTORYCOMMAND_HPP_
#define INVENTORYCOMMAND_HPP_

#include "application/Command/AI/IAiCommandElement.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class InventoryCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !INVENTORYCOMMAND_HPP_ */
