/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** EjectCommand.cpp
*/

#include "Command/AI/EjectCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void EjectCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("EjectCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_eject([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] Server &server)
{
    try {
        EjectCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_eject failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] EjectCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
