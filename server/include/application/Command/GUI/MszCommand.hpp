/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MszCommand.hpp
*/

#ifndef MSZCOMMAND_HPP_
#define MSZCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class MszCommand
 * @ingroup visitor_gui
 * @brief GUI "msz" command DTO (map size).
 * @par Protocol: Request "msz\n" -> Response "msz X Y\n".
 */
class MszCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(MszCommand*). */
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

private:
    std::shared_ptr<GUI> _client;
};

#endif /* !MSZCOMMAND_HPP_ */
