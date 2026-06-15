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

#endif /* !IAIVISITOR_HPP_ */
