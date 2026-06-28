/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BaseGuiCommand.hpp
*/

/**
 * @file BaseGuiCommand.hpp
 * @brief Common base for GUI command DTOs (holds the requesting client).
 */

#ifndef BASEGUICOMMAND_HPP_
#define BASEGUICOMMAND_HPP_

#include "application/Command/GUI/IGuiCommandElement.hpp"
#include <memory>

#include "domain/Client/GUI.hpp"

/**
 * @class BaseGuiCommand
 * @ingroup visitor_gui
 * @brief Shared storage of the issuing GUI client for command DTOs.
 */
class BaseGuiCommand : public IGuiCommandElement {
public:
    virtual ~BaseGuiCommand() = default;

    /** @brief Sets the GUI client that issued the command. */
    void setClient(std::shared_ptr<GUI> client);
    /** @brief Returns the issuing GUI client. */
    std::shared_ptr<GUI> getClient() const;

protected:
    std::shared_ptr<GUI> _client; /**< Issuing GUI client. */
};

#endif /* !BASEGUICOMMAND_HPP_ */
