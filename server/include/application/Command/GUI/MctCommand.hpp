/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MctCommand.hpp
*/

#ifndef MCTCOMMAND_HPP_
#define MCTCOMMAND_HPP_

#include "application/Command/GUI/BaseGuiCommand.hpp"
#include <memory>

class MctCommand : public BaseGuiCommand {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

private:
    std::shared_ptr<GUI> _client;
};

#endif /* !MCTCOMMAND_HPP_ */
