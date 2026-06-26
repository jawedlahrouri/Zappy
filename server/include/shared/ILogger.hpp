/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ILogger.hpp
*/

/**
 * @file ILogger.hpp
 * @brief Logging interface used throughout the server.
 */

#pragma once

#include <string>

/**
 * @interface ILogger
 * @brief Severity-based logging contract (Dependency Inversion).
 *
 * Injected wherever logging is needed so the sink (console, file, test capture)
 * can be swapped without touching call sites. @ref ConsoleLogger is the default.
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /** @brief Logs an informational message. */
    virtual void info(const std::string& message) = 0;
    /** @brief Logs an error message. */
    virtual void error(const std::string& message) = 0;
    /** @brief Logs a warning message. */
    virtual void warn(const std::string& message) = 0;
    /** @brief Logs a debug message. */
    virtual void debug(const std::string& message) = 0;
};
