/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ForkCommand.hpp
*/

#ifndef FORKCOMMAND_HPP_
#define FORKCOMMAND_HPP_

#include "Command/AI/IAiCommandElement.hpp"
#include "domain/Client/AI.hpp"
#include "application/Command/AI/IAiVisitor.hpp"

class ForkCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !FORKCOMMAND_HPP_ */
