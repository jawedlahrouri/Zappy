/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** LeftCommand.cpp
*/

#include "Command/AI/LeftCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void LeftCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("LeftCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_left(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        LeftCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_left failed: ") + e.what());
    }
}

void AiVisitor::visit(LeftCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
