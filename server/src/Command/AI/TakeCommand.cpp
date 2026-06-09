/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TakeCommand.cpp
*/

#include "Command/AI/TakeCommand.hpp"
#include "Command/AI/AiVisitor.hpp"
#include "Parsing/AiCommandParser.hpp"
#include "Server/Server.hpp"
#include "Parsing/Respond.hpp"
#include "Exception_Server.hpp"
#include <iostream>

void TakeCommand::Accept(IAiVisitor *visitor) {
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("TakeCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_take([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] Server &server)
{
    try {
        TakeCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_take failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] TakeCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
