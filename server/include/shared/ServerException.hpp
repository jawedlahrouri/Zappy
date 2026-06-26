/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** ServerException.hpp
*/

/**
 * @file ServerException.hpp
 * @brief Exception type used across the server.
 */

#pragma once

#include <exception>
#include <string>

/**
 * @class ServerException
 * @brief Carries a contextual message; thrown (and wrapped) across layers.
 *
 * Handlers typically rethrow with a prefixed context (e.g. "Server::foo: ...")
 * so the final @c what() reads as a call trace.
 */
class ServerException : public std::exception {
    std::string message; /**< Human-readable error message. */
public:
    /** @brief Builds the exception with a descriptive message. */
    ServerException(const std::string& msg) : message(msg) {}
    /** @brief Returns the stored message. */
    const char* what() const noexcept override {
        return message.c_str();
    }
};