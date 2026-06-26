/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EventVisitor.hpp
*/

/**
 * @file EventVisitor.hpp
 * @brief Concrete visitor serialising GUI events onto the wire.
 */

#ifndef EVENTVISITOR_HPP_
#define EVENTVISITOR_HPP_

#include "IEventVisitor.hpp"
#include "INetworkProvider.hpp"

/**
 * @class EventVisitor
 * @ingroup visitor_event
 * @brief Concrete @ref IEventVisitor formatting events for the GUI protocol.
 *
 * Each @c visit() builds the textual event and sends it either to a single GUI
 * (when @ref setTargetClient was called, e.g. the initial snapshot for a
 * freshly connected GUI) or, by default, broadcast to every GUI.
 */
class EventVisitor : public IEventVisitor {
public:
    /**
     * @brief Builds the visitor over the network used to emit events.
     * @param network Provider for sending to one or all GUI clients.
     */
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

    /**
     * @brief Restricts the next events to a single GUI fd instead of all GUIs.
     * @param fd Target client fd (used for per-client initial snapshots).
     */
    void setTargetClient(int fd);

private:
    INetworkProvider &_network; /**< Injected network used to emit events. */
    int _targetFd = -1;         /**< Single target fd, or -1 to broadcast. */
};

#endif /* !EVENTVISITOR_HPP_ */
