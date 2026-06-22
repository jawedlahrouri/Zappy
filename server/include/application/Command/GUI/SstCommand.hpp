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

class SstCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;

    void setNewFreq(int freq);
    int getNewFreq() const;

private:
    int _newFreq = 0;
};

#endif /* !SSTCOMMAND_HPP_ */