/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LookCommand.hpp
*/

#ifndef LOOKCOMMAND_HPP_
#define LOOKCOMMAND_HPP_

#include "Command/AI/IAiCommandElement.hpp"
#include <memory>
class AI;

class IAiVisitor;

class LookCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !LOOKCOMMAND_HPP_ */
