/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.hpp
*/

#ifndef PPOCOMMAND_HPP_
#define PPOCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class PpoCommand
 * @ingroup visitor_gui
 * @brief GUI "ppo" command DTO (player position).
 * @par Protocol: Request "ppo #n\n" -> Response "ppo #n X Y O\n".
 */
class PpoCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(PpoCommand*). */
    void Accept(IGuiVisitor *visitor) override;

    /** @brief Sets the queried player id. */
    void setPlayerId(int id);
    /** @brief Returns the queried player id. */
    int getPlayerId() const;

private:
    int _playerId = 0; /**< Queried player id. */
};

#endif /* !PPOCOMMAND_HPP_ */
