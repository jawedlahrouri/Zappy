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
    EventVisitor(INetworkProvider &network) : _network(network) {}
    ~EventVisitor() override = default;

    void visit(PdiEvent *event) override;
    void visit(PdrEvent *event) override;
    void visit(PnwEvent *event) override;
    void visit(SegEvent *event) override;
    void visit(EnwEvent *event) override;
    void visit(PexEvent *event) override;
    void visit(PieEvent *event) override;

private:
    INetworkProvider &_network;
};

#endif /* !EVENTVISITOR_HPP_ */
