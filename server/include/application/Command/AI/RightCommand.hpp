/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RightCommand.hpp
*/

#ifndef RIGHTCOMMAND_HPP_
#define RIGHTCOMMAND_HPP_

#include "Command/AI/IAiCommandElement.hpp"
#include <memory>
class AI;

class IAiVisitor;

class RightCommand : public IAiCommandElement {
public:
    void Accept(IAiVisitor *visitor) override;
    void setClient(std::shared_ptr<AI> client) { _client = client; }
    std::shared_ptr<AI> getClient() const { return _client; }
private:
    std::shared_ptr<AI> _client;
};

#endif /* !RIGHTCOMMAND_HPP_ */
