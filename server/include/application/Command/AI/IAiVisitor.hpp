/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IAiVisitor
*/

/**
 * @file IAiVisitor.hpp
 * @brief Visitor interface for the AI protocol commands.
 */

/**
 * @defgroup visitor_ai AI Command Visitor
 * @brief Double-dispatch over the AI protocol commands (Visitor pattern).
 *
 * @par Design Pattern: Visitor (GoF)
 * Separates the AI command *objects* (lightweight DTOs carrying the parsed
 * request) from the game logic that *executes* them. Adding a new command means
 * adding one element class and one @c visit() overload, without editing the
 * existing commands.
 * - Visitor interface : @ref IAiVisitor — one @c visit() overload per command.
 * - Concrete visitor  : @ref AiVisitor — runs the game logic and replies to the
 *   client through an injected @c INetworkProvider.
 * - Element interface  : @ref IAiCommandElement — declares @c Accept(IAiVisitor*).
 * - Concrete elements  : @c ForwardCommand, @c LookCommand, ... (the DTOs).
 *
 * @par Flow (double dispatch)
 * The parser fills a DTO, then calls @c dto.Accept(&visitor); @c Accept calls
 * back @c visitor.visit(this), so the right overload is selected from both the
 * element's and the visitor's dynamic types.
 * @{
 */

#ifndef IAIVISITOR_HPP_
#define IAIVISITOR_HPP_

#include "application/Command/AI/ForwardCommand.hpp"
#include "application/Command/AI/RightCommand.hpp"
#include "application/Command/AI/LeftCommand.hpp"
#include "application/Command/AI/LookCommand.hpp"
#include "application/Command/AI/InventoryCommand.hpp"
#include "application/Command/AI/BroadcastCommand.hpp"
#include "application/Command/AI/ConnectNbrCommand.hpp"
#include "application/Command/AI/ForkCommand.hpp"
#include "application/Command/AI/EjectCommand.hpp"
#include "application/Command/AI/TakeCommand.hpp"
#include "application/Command/AI/SetCommand.hpp"
#include "application/Command/AI/IncantationCommand.hpp"

/**
 * @interface IAiVisitor
 * @ingroup visitor_ai
 * @brief Declares one @c visit() handler per concrete AI command.
 */
class IAiVisitor {
public:
    virtual ~IAiVisitor() = default;
    virtual void visit(class ForwardCommand *element) = 0;
    virtual void visit(class RightCommand *element) = 0;
    virtual void visit(class LeftCommand *element) = 0;
    virtual void visit(class LookCommand *element) = 0;
    virtual void visit(class InventoryCommand *element) = 0;
    virtual void visit(class BroadcastCommand *element) = 0;
    virtual void visit(class ConnectNbrCommand *element) = 0;
    virtual void visit(class ForkCommand *element) = 0;
    virtual void visit(class EjectCommand *element) = 0;
    virtual void visit(class TakeCommand *element) = 0;
    virtual void visit(class SetCommand *element) = 0;
    virtual void visit(class IncantationCommand *element) = 0;
};

/** @} */ // end of visitor_ai

#endif /* !IAIVISITOR_HPP_ */
