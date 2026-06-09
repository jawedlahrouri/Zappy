/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IEventVisitor.hpp
*/

#ifndef IEVENTVISITOR_HPP_
#define IEVENTVISITOR_HPP_

class PdiEvent;
class PdrEvent;
class PnwEvent;

class IEventVisitor {
public:
    virtual ~IEventVisitor() = default;
    virtual void visit(PdiEvent *event) = 0;
    virtual void visit(PdrEvent *event) = 0;
    virtual void visit(PnwEvent *event) = 0;
};

#endif /* !IEVENTVISITOR_HPP_ */
