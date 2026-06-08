#ifndef IGUIVISITOR_HPP_
#define IGUIVISITOR_HPP_

#include "GUI/MszCommand.hpp"
#include "GUI/BctCommand.hpp"
#include "GUI/MctCommand.hpp"
#include "GUI/TnaCommand.hpp"
#include "GUI/PpoCommand.hpp"
#include "GUI/PlvCommand.hpp"
#include "GUI/PinCommand.hpp"
#include "GUI/PicCommand.hpp"
#include "GUI/PieCommand.hpp"
#include "GUI/PdrCommand.hpp"
#include "GUI/PgtCommand.hpp"
#include "GUI/PbcCommand.hpp"
#include "GUI/PfcCommand.hpp"
#include "GUI/PfkCommand.hpp"
#include "GUI/SgtCommand.hpp"
#include "GUI/SstCommand.hpp"

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
    virtual void visit(PicCommand *element) = 0;
    virtual void visit(PieCommand *element) = 0;
    virtual void visit(PdrCommand *element) = 0;
    virtual void visit(PgtCommand *element) = 0;
    virtual void visit(PbcCommand *element) = 0;
    virtual void visit(PfcCommand *element) = 0;
    virtual void visit(PfkCommand *element) = 0;
    virtual void visit(SgtCommand *element) = 0;
    virtual void visit(SstCommand *element) = 0;
};

#endif /* !IGUIVISITOR_HPP_ */
