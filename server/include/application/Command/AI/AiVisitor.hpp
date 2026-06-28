/**
 * @file AiVisitor.hpp
 * @brief Concrete visitor executing AI commands and replying to the client.
 */

#ifndef AIVISITOR_HPP_
#define AIVISITOR_HPP_

#include "IAiVisitor.hpp"
#include "INetworkProvider.hpp"

/**
 * @class AiVisitor
 * @ingroup visitor_ai
 * @brief Concrete @ref IAiVisitor that runs each AI command's game logic.
 *
 * Each @c visit() overload reads the parsed data from its element, applies the
 * effect on the game world and sends the protocol response through the injected
 * @c INetworkProvider (Dependency Injection — the visitor never owns the
 * network, which keeps it testable with a stub provider).
 */
class AiVisitor : public IAiVisitor {
public:
    /**
     * @brief Builds the visitor over the network used to reply and emit events.
     * @param network Provider for client responses and GUI event broadcasting.
     */
    AiVisitor(INetworkProvider &network) : _network(network) {}
    ~AiVisitor() override = default;

    void visit(ForwardCommand *element) override;
    void visit(RightCommand *element) override;
    void visit(LeftCommand *element) override;
    void visit(LookCommand *element) override;
    void visit(InventoryCommand *element) override;
    void visit(BroadcastCommand *element) override;
    void visit(ConnectNbrCommand *element) override;
    void visit(ForkCommand *element) override;
    void visit(EjectCommand *element) override;
    void visit(TakeCommand *element) override;
    void visit(SetCommand *element) override;
    void visit(IncantationCommand *element) override;

private:
    INetworkProvider &_network; /**< Injected network used to reply/emit events. */
};

#endif /* !AIVISITOR_HPP_ */
