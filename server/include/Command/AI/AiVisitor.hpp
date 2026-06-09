#ifndef AIVISITOR_HPP_
#define AIVISITOR_HPP_

#include "IAiVisitor.hpp"

class Server;

class AiVisitor : public IAiVisitor {
public:
    AiVisitor(Server &server) : _server(server) {}
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
    Server &_server;
};

#endif /* !AIVISITOR_HPP_ */
