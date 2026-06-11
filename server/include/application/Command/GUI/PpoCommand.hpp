/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PpoCommand.hpp
*/

#ifndef PPOCOMMAND_HPP_
#define PPOCOMMAND_HPP_

#include "application/Command/GUI/IGuiCommandElement.hpp"
#include <memory>
class GUI;

class IGuiVisitor;

class PpoCommand : public IGuiCommandElement {
public:
    void Accept(IGuiVisitor *visitor) override;
    void setClient(std::shared_ptr<GUI> client) { _client = client; }
    std::shared_ptr<GUI> getClient() const { return _client; }

    void setPlayerId(int id) { _playerId = id; }
    int getPlayerId() const { return _playerId; }

private:
    std::shared_ptr<GUI> _client;
    int _playerId = -1;
};

#endif /* !PPOCOMMAND_HPP_ */
