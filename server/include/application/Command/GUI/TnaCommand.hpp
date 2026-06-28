/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TnaCommand.hpp
*/

#ifndef TNACOMMAND_HPP_
#define TNACOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

/**
 * @class TnaCommand
 * @ingroup visitor_gui
 * @brief GUI "tna" command DTO (team names).
 * @par Protocol: Request "tna\n" -> Response one "tna N\n" per team.
 */
class TnaCommand : public BaseGuiCommand {
public:
    /** @brief Dispatches to @c IGuiVisitor::visit(TnaCommand*). */
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

private:
    std::shared_ptr<GUI> _client;
};

#endif /* !TNACOMMAND_HPP_ */
