/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ServerException.hpp
*/

#pragma once

#include <exception>
#include <string>

class ServerException : public std::exception {
    std::string message;
public:
    ServerException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
}; 