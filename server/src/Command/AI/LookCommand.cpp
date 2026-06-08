/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LookCommand.cpp
*/

#include "Command/AI/LookCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void LookCommand::Accept(IAiVisitor *visitor) {
    visitor->visit(this);
}

void AiCommandParser::parse_look(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        LookCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_look failed: ") + e.what());
    }
}

void AiVisitor::visit(LookCommand *element) {
    (void)element;
}
