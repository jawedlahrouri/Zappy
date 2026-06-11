/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.hpp
*/

#ifndef MCTCOMMAND_HPP_
#define MCTCOMMAND_HPP_

#include "application/Command/GUI/IGuiCommandElement.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class MctCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client) { _client = client; }
    std::shared_ptr<GUI> getClient() const { return _client; }
private:
    std::shared_ptr<GUI> _client;
};

#endif /* !MCTCOMMAND_HPP_ */
