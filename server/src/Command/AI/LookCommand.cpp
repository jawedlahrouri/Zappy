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
    try {
    visitor->visit(this);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("LookCommand::Accept: ") + e.what());
    }
}

void AiCommandParser::parse_look([[maybe_unused]] std::shared_ptr<AI> client, [[maybe_unused]] const std::string &command, [[maybe_unused]] Server &server)
{
    try {
        LookCommand cmd;
        AiVisitor visitor(server);
        cmd.Accept(&visitor);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("parse_look failed: ") + e.what());
    }
}

void AiVisitor::visit([[maybe_unused]] LookCommand *element) {
    try {
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("AiVisitor::visit: ") + e.what());
    }
}
