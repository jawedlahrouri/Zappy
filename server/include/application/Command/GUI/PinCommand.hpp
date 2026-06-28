/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PinCommand.hpp
*/

#ifndef PINCOMMAND_HPP_
#define PINCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class PinCommand
 * @ingroup visitor_gui
 * @brief GUI "pin" command DTO (player inventory).
 * @par Protocol: Request "pin #n\n" -> Response "pin #n X Y q0 q1 q2 q3 q4 q5 q6\n".
 */
class PinCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(PinCommand*). */
    void Accept(IGuiVisitor *visitor) override;

    /** @brief Sets the queried player id. */
    void setPlayerId(int id);
    /** @brief Returns the queried player id. */
    int getPlayerId() const;

private:
    int _playerId = 0; /**< Queried player id. */
};

#endif /* !PINCOMMAND_HPP_ */