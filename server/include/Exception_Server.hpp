/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Exception_Server.hpp
*/

#include <exception>
#include <string>

class Exception_Server : public std::exception {
    std::string message;
public:
    Exception_Server(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
}; 