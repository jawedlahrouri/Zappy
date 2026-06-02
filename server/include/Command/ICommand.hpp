/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ICommand.hpp
*/

#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(std::string args) = 0;
};