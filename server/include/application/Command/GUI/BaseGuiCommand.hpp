/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** BaseGuiCommand.hpp
*/

#ifndef BASEGUICOMMAND_HPP_
#define BASEGUICOMMAND_HPP_

#include "application/Command/GUI/IGuiCommandElement.hpp"
#include <memory>

#include "domain/Client/GUI.hpp"

class BaseGuiCommand : public IGuiCommandElement {
public:
    virtual ~BaseGuiCommand() = default;

    void setClient(std::shared_ptr<GUI> client);
    std::shared_ptr<GUI> getClient() const;

protected:
    std::shared_ptr<GUI> _client;
};

#endif /* !BASEGUICOMMAND_HPP_ */
