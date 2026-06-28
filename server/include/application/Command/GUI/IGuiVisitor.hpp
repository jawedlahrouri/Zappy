/**
 * @file IGuiVisitor.hpp
 * @brief Visitor interface for the GUI protocol commands.
 */

/**
 * @defgroup visitor_gui GUI Command Visitor
 * @brief Double-dispatch over the GUI protocol commands (Visitor pattern).
 *
 * @par Design Pattern: Visitor (GoF)
 * Same shape as the AI command visitor (@ref visitor_ai), applied to the
 * commands a GUI sends (map size, tile content, time unit, ...):
 * - Visitor interface : @ref IGuiVisitor — one @c visit() per GUI command.
 * - Concrete visitor  : @ref GuiVisitor — builds and sends the GUI reply.
 * - Element interface  : @ref IGuiCommandElement — declares @c Accept(IGuiVisitor*).
 * - Concrete elements  : @c MszCommand, @c BctCommand, ... (the DTOs).
 *
 * Protocol: the Zappy GUI protocol.
 * @{
 */

#ifndef IGUIVISITOR_HPP_
#define IGUIVISITOR_HPP_

#include "application/Command/GUI/MszCommand.hpp"
#include "application/Command/GUI/BctCommand.hpp"
#include "application/Command/GUI/MctCommand.hpp"
#include "application/Command/GUI/TnaCommand.hpp"
#include "application/Command/GUI/PpoCommand.hpp"
#include "application/Command/GUI/PlvCommand.hpp"
#include "application/Command/GUI/PinCommand.hpp"

#include "application/Command/GUI/SgtCommand.hpp"
#include "application/Command/GUI/SstCommand.hpp"

/**
 * @interface IGuiVisitor
 * @ingroup visitor_gui
 * @brief Declares one @c visit() handler per concrete GUI command.
 */
class IGuiVisitor {
public:
    virtual ~IGuiVisitor() = default;
    virtual void visit(MszCommand *element) = 0;
    virtual void visit(BctCommand *element) = 0;
    virtual void visit(MctCommand *element) = 0;
    virtual void visit(TnaCommand *element) = 0;
    virtual void visit(PpoCommand *element) = 0;
    virtual void visit(PlvCommand *element) = 0;
    virtual void visit(PinCommand *element) = 0;

    virtual void visit(SgtCommand *element) = 0;
    virtual void visit(SstCommand *element) = 0;
};

/** @} */ // end of visitor_gui

#endif /* !IGUIVISITOR_HPP_ */
