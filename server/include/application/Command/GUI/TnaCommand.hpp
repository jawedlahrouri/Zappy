/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TnaCommand.hpp
*/

#ifndef TNACOMMAND_HPP_
#define TNACOMMAND_HPP_

#include "application/Command/GUI/IGuiCommandElement.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class TnaCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

private:
    std::shared_ptr<GUI> _client;
};

#endif /* !TNACOMMAND_HPP_ */
