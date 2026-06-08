/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PicCommand.hpp
*/

#ifndef PICCOMMAND_HPP_
#define PICCOMMAND_HPP_

#include "Command/GUI/IGuiCommandElement.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class PicCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client) { _client = client; }
    std::shared_ptr<GUI> getClient() const { return _client; }
private:
    std::shared_ptr<GUI> _client;
};

#endif /* !PICCOMMAND_HPP_ */
