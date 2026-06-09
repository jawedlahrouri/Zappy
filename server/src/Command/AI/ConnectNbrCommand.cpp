/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConnectNbrCommand.cpp
*/

#include "Command/AI/ConnectNbrCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void ConnectNbrCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("ConnectNbrCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_connect_nbr(std::shared_ptr<AI> client, const std::string &command, Server &server)
{
    try {
        (void)client;
        (void)command;
        (void)server;
        ConnectNbrCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_connect_nbr failed: ") + e.what());
    }
}

void AiVisitor::visit(ConnectNbrCommand *element) {
    try {
    (void)element;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
