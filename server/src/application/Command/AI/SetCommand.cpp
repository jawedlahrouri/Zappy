/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SetCommand.cpp
*/

#include "Command/AI/SetCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void SetCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("SetCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_set(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        SetCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_set failed: ") + e.what());
    }
}

void AiVisitor::visit(SetCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
