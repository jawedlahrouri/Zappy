#ifndef IAIVISITOR_HPP_
#define IAIVISITOR_HPP_

#include "Command/AI/ForwardCommand.hpp"
#include "Command/AI/RightCommand.hpp"
#include "Command/AI/LeftCommand.hpp"
#include "Command/AI/LookCommand.hpp"
#include "Command/AI/InventoryCommand.hpp"
#include "Command/AI/BroadcastCommand.hpp"
#include "Command/AI/ConnectNbrCommand.hpp"
#include "Command/AI/ForkCommand.hpp"
#include "Command/AI/EjectCommand.hpp"
#include "Command/AI/TakeCommand.hpp"
#include "Command/AI/SetCommand.hpp"
#include "Command/AI/IncantationCommand.hpp"

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
