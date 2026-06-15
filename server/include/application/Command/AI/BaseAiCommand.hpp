/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BaseAiCommand.hpp
*/

#ifndef BASEAICOMMAND_HPP_
#define BASEAICOMMAND_HPP_

#include "application/Command/AI/IAiCommandElement.hpp"
#include <memory>

#include "domain/Client/AI.hpp"

class BaseAiCommand : public IAiCommandElement {
public:
    virtual ~BaseAiCommand() = default;

    void setClient(std::shared_ptr<AI> client);
    std::shared_ptr<AI> getClient() const;
    int getX() const;
    int getY() const;

protected:
    std::shared_ptr<AI> _client;
};

#endif /* !BASEAICOMMAND_HPP_ */
