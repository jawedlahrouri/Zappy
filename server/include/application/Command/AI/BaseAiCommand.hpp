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

/**
 * @class BaseAiCommand
 * @ingroup visitor_ai
 * @brief Shared storage of the issuing AI client for command DTOs.
 */
class BaseAiCommand : public IAiCommandElement {
public:
    virtual ~BaseAiCommand() = default;

    /** @brief Sets the AI client that issued the command. */
    void setClient(std::shared_ptr<AI> client);
    /** @brief Returns the issuing AI client. */
    std::shared_ptr<AI> getClient() const;
    /** @brief Returns the issuing player's X coordinate. */
    int getX() const;
    /** @brief Returns the issuing player's Y coordinate. */
    int getY() const;

protected:
    std::shared_ptr<AI> _client; /**< Issuing AI client. */
};

#endif /* !BASEAICOMMAND_HPP_ */
