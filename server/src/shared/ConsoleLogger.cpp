/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConsoleLogger.cpp
*/

#include "shared/ConsoleLogger.hpp"
#include <iostream>

/** @brief Writes "[INFO] ..." to stdout. */
void ConsoleLogger::info(const std::string& message)
{
    std::cout << "[INFO] " << message << std::endl;
}

/** @brief Writes "[ERROR] ..." to stderr. */
void ConsoleLogger::error(const std::string& message)
{
    std::cerr << "[ERROR] " << message << std::endl;
}

/** @brief Writes "[WARN] ..." to stdout. */
void ConsoleLogger::warn(const std::string& message)
{
    std::cout << "[WARN] " << message << std::endl;
}

/** @brief Writes "[DEBUG] ..." to stdout. */
void ConsoleLogger::debug(const std::string& message)
{
    std::cout << "[DEBUG] " << message << std::endl;
}
