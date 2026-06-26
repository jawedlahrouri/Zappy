/**
 * @file GuiVisitor.hpp
 * @brief Concrete visitor answering GUI commands.
 */

#ifndef GUIVISITOR_HPP_
#define GUIVISITOR_HPP_

#include "IGuiVisitor.hpp"
#include "INetworkProvider.hpp"

/**
 * @class GuiVisitor
 * @ingroup visitor_gui
 * @brief Concrete @ref IGuiVisitor that builds and sends each GUI reply.
 *
 * Reads the requested data from the game via the injected @c INetworkProvider
 * and formats the protocol response for the requesting GUI client.
 */
class GuiVisitor : public IGuiVisitor {
public:
    /**
     * @brief Builds the visitor over the network used to reply.
     * @param network Provider for game state and client responses.
     */
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
    INetworkProvider &_network; /**< Injected network used to reply. */
};

#endif /* !GUIVISITOR_HPP_ */
