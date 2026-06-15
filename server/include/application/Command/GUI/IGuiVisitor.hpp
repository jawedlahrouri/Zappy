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
