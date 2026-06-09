/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EventVisitor.hpp
*/

#ifndef EVENTVISITOR_HPP_
#define EVENTVISITOR_HPP_

#include "IEventVisitor.hpp"

class Server;

class EventVisitor : public IEventVisitor {
public:
    EventVisitor(Server &server) : _server(server) {}
    ~EventVisitor() override = default;

    void visit(PdiEvent *event) override;
    void visit(PdrEvent *event) override;
    void visit(PnwEvent *event) override;

private:
    Server &_server;
};

#endif /* !EVENTVISITOR_HPP_ */
