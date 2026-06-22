/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConsoleLogger.cpp
*/

#include "shared/ConsoleLogger.hpp"
#include <iostream>

void ConsoleLogger::info(const std::string& message)
{
    std::cout << "[INFO] " << message << std::endl;
}

void ConsoleLogger::error(const std::string& message)
{
    std::cerr << "[ERROR] " << message << std::endl;
}

void ConsoleLogger::warn(const std::string& message)
{
    std::cout << "[WARN] " << message << std::endl;
}

void ConsoleLogger::debug(const std::string& message)
{
    std::cout << "[DEBUG] " << message << std::endl;
}
