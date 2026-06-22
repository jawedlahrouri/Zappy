/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EventVisitor.hpp
*/

#ifndef EVENTVISITOR_HPP_
#define EVENTVISITOR_HPP_

#include "IEventVisitor.hpp"
#include "INetworkProvider.hpp"

class EventVisitor : public IEventVisitor {
public:
    EventVisitor(INetworkProvider &network);
    ~EventVisitor() override = default;

    void visit(PdiEvent *event) override;
    void visit(PdrEvent *event) override;
    void visit(PnwEvent *event) override;
    void visit(SegEvent *event) override;
    void visit(EnwEvent *event) override;
    void visit(PexEvent *event) override;
    void visit(PieEvent *event) override;
    void visit(BctEvent *event) override;
    void visit(PbcEvent *event) override;
    void visit(PicEvent *event) override;
    void visit(PfkEvent *event) override;
    void visit(PpoEvent *event) override;
    void visit(PgtEvent *event) override;
    void visit(PinEvent *event) override;
    void visit(EdiEvent *event) override;
    void visit(EboEvent *event) override;

    void setTargetClient(int fd);

private:
    INetworkProvider &_network;
    int _targetFd = -1;
};

#endif /* !EVENTVISITOR_HPP_ */
