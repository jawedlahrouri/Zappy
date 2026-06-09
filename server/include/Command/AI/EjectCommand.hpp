/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EjectCommand.hpp
*/

#ifndef EJECTCOMMAND_HPP_
#define EJECTCOMMAND_HPP_

#include "Command/AI/IAiCommandElement.hpp"
#include <memory>
class AI;

class IAiVisitor;

class EjectCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !EJECTCOMMAND_HPP_ */
