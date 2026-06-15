/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IncantationCommand.hpp
*/

#ifndef INCANTATIONCOMMAND_HPP_
#define INCANTATIONCOMMAND_HPP_

#include "application/Command/AI/IAiCommandElement.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class IncantationCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !INCANTATIONCOMMAND_HPP_ */
