#ifndef IGUIVISITOR_HPP_
#define IGUIVISITOR_HPP_

#include "Command/GUI/MszCommand.hpp"
#include "Command/GUI/BctCommand.hpp"
#include "Command/GUI/MctCommand.hpp"
#include "Command/GUI/TnaCommand.hpp"
#include "Command/GUI/PpoCommand.hpp"
#include "Command/GUI/PlvCommand.hpp"
#include "Command/GUI/PinCommand.hpp"

#include "Command/GUI/SgtCommand.hpp"
#include "Command/GUI/SstCommand.hpp"

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

#endif /* !IGUIVISITOR_HPP_ */
