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
    virtual void visit(ForwardCommand *element) = 0;
    virtual void visit(RightCommand *element) = 0;
    virtual void visit(LeftCommand *element) = 0;
    virtual void visit(LookCommand *element) = 0;
    virtual void visit(InventoryCommand *element) = 0;
    virtual void visit(BroadcastCommand *element) = 0;
    virtual void visit(ConnectNbrCommand *element) = 0;
    virtual void visit(ForkCommand *element) = 0;
    virtual void visit(EjectCommand *element) = 0;
    virtual void visit(TakeCommand *element) = 0;
    virtual void visit(SetCommand *element) = 0;
    virtual void visit(IncantationCommand *element) = 0;
};

#endif /* !IAIVISITOR_HPP_ */
