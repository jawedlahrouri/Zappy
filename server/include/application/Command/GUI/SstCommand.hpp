/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SstCommand.hpp
*/

#ifndef SSTCOMMAND_HPP_
#define SSTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class SstCommand
 * @ingroup visitor_gui
 * @brief GUI "sst" command DTO (set time unit / frequency).
 * @par Protocol: Request "sst T\n" -> Response "sst T\n" (new frequency).
 */
class SstCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(SstCommand*). */
    void Accept(IGuiVisitor *visitor) override;

    /** @brief Sets the requested new frequency. */
    void setNewFreq(int freq);
    /** @brief Returns the requested new frequency. */
    int getNewFreq() const;

private:
    int _newFreq = 0; /**< Requested new frequency. */
};

#endif /* !SSTCOMMAND_HPP_ */