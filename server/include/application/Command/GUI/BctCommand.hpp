/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BctCommand.hpp
*/

/**
 * @file BctCommand.hpp
 * @brief DTO for the GUI "bct" command (content of one tile).
 */

#ifndef BCTCOMMAND_HPP_
#define BCTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class GUI;
class IGuiVisitor;

/**
 * @class BctCommand
 * @ingroup visitor_gui
 * @brief GUI command DTO carrying the tile coordinates to inspect.
 *
 * @par Protocol: Request "bct X Y\n" -> Response "bct X Y q0 q1 q2 q3 q4 q5 q6\n".
 */
class BctCommand : public IGuiCommandElement {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(BctCommand*). */
    void Accept(IGuiVisitor *visitor) override;

    /** @brief Sets the issuing GUI client. */
    void setClient(std::shared_ptr<GUI> client);
    /** @brief Returns the issuing GUI client. */
    std::shared_ptr<GUI> getClient() const;

    /** @brief Sets the tile X coordinate. */
    void setX(int x);
    /** @brief Sets the tile Y coordinate. */
    void setY(int y);

    /** @brief Returns the tile X coordinate. */
    int getX() const;
    /** @brief Returns the tile Y coordinate. */
    int getY() const;

private:
    std::shared_ptr<GUI> _client; /**< Issuing GUI client. */
    int _x = 0;                   /**< Tile X coordinate. */
    int _y = 0;                   /**< Tile Y coordinate. */
};

#endif /* !BCTCOMMAND_HPP_ */