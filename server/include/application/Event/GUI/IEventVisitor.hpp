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
class BctEvent;
class PbcEvent;
class PicEvent;
class PfkEvent;
class PpoEvent;
class PgtEvent;
class PinEvent;
class EdiEvent;
class EboEvent;

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
    virtual void visit(BctEvent *event) = 0;
    virtual void visit(PbcEvent *event) = 0;
    virtual void visit(PicEvent *event) = 0;
    virtual void visit(PfkEvent *event) = 0;
    virtual void visit(PpoEvent *event) = 0;
    virtual void visit(PgtEvent *event) = 0;
    virtual void visit(PinEvent *event) = 0;
    virtual void visit(EdiEvent *event) = 0;
    virtual void visit(EboEvent *event) = 0;
};

#endif /* !IEVENTVISITOR_HPP_ */
