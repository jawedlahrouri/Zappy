/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PfkCommand.hpp
*/

#ifndef PFKCOMMAND_HPP_
#define PFKCOMMAND_HPP_

#include "Command/GUI/IGuiCommandElement.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class PfkCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client) { _client = client; }
    std::shared_ptr<GUI> getClient() const { return _client; }
private:
    std::shared_ptr<GUI> _client;
};

#endif /* !PFKCOMMAND_HPP_ */
