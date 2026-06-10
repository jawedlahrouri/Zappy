#include "Network/INetworkProvider.hpp"
#ifndef GUIVISITOR_HPP_
#define GUIVISITOR_HPP_

#include "IGuiVisitor.hpp"

class INetworkProvider;

class GuiVisitor : public IGuiVisitor {
public:
    GuiVisitor(INetworkProvider &network) : _network(network) {}
    ~GuiVisitor() override = default;

    void visit(MszCommand *element) override;
    void visit(BctCommand *element) override;
    void visit(MctCommand *element) override;
    void visit(TnaCommand *element) override;
    void visit(PpoCommand *element) override;
    void visit(PlvCommand *element) override;
    void visit(PinCommand *element) override;

    void visit(SgtCommand *element) override;
    void visit(SstCommand *element) override;

private:
    INetworkProvider &_network;
};

#endif /* !GUIVISITOR_HPP_ */
