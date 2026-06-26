/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SgtCommand.hpp
*/

#ifndef SGTCOMMAND_HPP_
#define SGTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class SgtCommand
 * @ingroup visitor_gui
 * @brief GUI "sgt" command DTO (get time unit).
 * @par Protocol: Request "sgt\n" -> Response "sgt T\n".
 */
class SgtCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(SgtCommand*). */
    void Accept(IGuiVisitor *visitor) override;
};

#endif /* !SGTCOMMAND_HPP_ */
