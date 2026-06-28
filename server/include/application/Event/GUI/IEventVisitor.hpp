/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IEventVisitor.hpp
*/

/**
 * @file IEventVisitor.hpp
 * @brief Visitor interface for server-to-GUI events.
 */

/**
 * @defgroup visitor_event GUI Event Visitor
 * @brief Double-dispatch over outgoing GUI events (Visitor pattern).
 *
 * @par Design Pattern: Visitor (GoF)
 * The third Visitor of the project (alongside @ref visitor_ai and
 * @ref visitor_gui), this one serialises the events the server *pushes* to GUIs
 * (player movement, broadcast, incantation, egg laid, ...):
 * - Visitor interface : @ref IEventVisitor — one @c visit() per event type.
 * - Concrete visitor  : @ref EventVisitor — formats the event and sends it to
 *   one GUI (@ref EventVisitor::setTargetClient) or all GUIs.
 * - Element interface  : @ref IEventElement — declares @c Accept(IEventVisitor*) const.
 * - Concrete elements  : @c PnwEvent, @c PbcEvent, ... (the event DTOs).
 *
 * Protocol: the Zappy GUI protocol.
 * @{
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

/**
 * @interface IEventVisitor
 * @ingroup visitor_event
 * @brief Declares one @c visit() handler per concrete GUI event.
 */
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

/** @} */ // end of visitor_event

#endif /* !IEVENTVISITOR_HPP_ */
