/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlvCommand.hpp
*/

#ifndef PLVCOMMAND_HPP_
#define PLVCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class PlvCommand
 * @ingroup visitor_gui
 * @brief GUI "plv" command DTO (player level).
 * @par Protocol: Request "plv #n\n" -> Response "plv #n L\n".
 */
class PlvCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(PlvCommand*). */
    void Accept(IGuiVisitor *visitor) override;

    /** @brief Sets the queried player id. */
    void setPlayerId(int id);
    /** @brief Returns the queried player id. */
    int getPlayerId() const;

private:
    int _playerId = 0; /**< Queried player id. */
};

#endif /* !PLVCOMMAND_HPP_ */