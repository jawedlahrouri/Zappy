/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ConsoleLogger.hpp
*/

#pragma once

#include "shared/ILogger.hpp"

class ConsoleLogger : public ILogger {
public:
    ConsoleLogger() = default;
    ~ConsoleLogger() override = default;

    void info(const std::string& message) override;
    void error(const std::string& message) override;
    void warn(const std::string& message) override;
    void debug(const std::string& message) override;
};
