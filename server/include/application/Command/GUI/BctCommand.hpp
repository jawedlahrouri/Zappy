/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BctCommand.hpp
*/

#ifndef BCTCOMMAND_HPP_
#define BCTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class GUI;
class IGuiVisitor;

class BctCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    
    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

    void setX(int x);
    void setY(int y);

    int getX() const;
    int getY() const;

private:
    std::shared_ptr<GUI> _client;
    int _x = 0;
    int _y = 0;
};

#endif /* !BCTCOMMAND_HPP_ */