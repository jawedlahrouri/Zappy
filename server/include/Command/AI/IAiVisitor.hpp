#ifndef IAIVISITOR_HPP_
#define IAIVISITOR_HPP_

#include "AI/ForwardCommand.hpp"
#include "AI/RightCommand.hpp"
#include "AI/LeftCommand.hpp"
#include "AI/LookCommand.hpp"
#include "AI/InventoryCommand.hpp"
#include "AI/BroadcastCommand.hpp"
#include "AI/ConnectNbrCommand.hpp"
#include "AI/ForkCommand.hpp"
#include "AI/EjectCommand.hpp"
#include "AI/TakeCommand.hpp"
#include "AI/SetCommand.hpp"
#include "AI/IncantationCommand.hpp"

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
