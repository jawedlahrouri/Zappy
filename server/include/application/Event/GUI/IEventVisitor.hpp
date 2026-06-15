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
class SegEvent;
class EnwEvent;
class PexEvent;
class PieEvent;

class IEventVisitor {
public:
    virtual ~IEventVisitor() = default;
    virtual void visit(PdiEvent *event) = 0;
    virtual void visit(PdrEvent *event) = 0;
    virtual void visit(PnwEvent *event) = 0;
    virtual void visit(SegEvent *event) = 0;
    virtual void visit(EnwEvent *event) = 0;
    virtual void visit(PexEvent *event) = 0;
    virtual void visit(PieEvent *event) = 0;
};

#endif /* !IEVENTVISITOR_HPP_ */
