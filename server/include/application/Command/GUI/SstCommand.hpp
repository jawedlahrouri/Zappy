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
class GUI;

class IGuiVisitor;

class SstCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

};

#endif /* !SSTCOMMAND_HPP_ */
