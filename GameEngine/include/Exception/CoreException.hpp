/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CoreException
*/

/**
 * @file CoreException.hpp
 * @brief Defines the exception used by the graphical client core.
 */

#ifndef COREEXCEPTION_HPP_
    #define COREEXCEPTION_HPP_

    #include <stdexcept>
    #include <string>

    /**
     * @brief Reports unrecoverable graphical client errors.
     */
    class CoreException : public std::runtime_error {
    public:
        explicit CoreException(const std::string& message)
            : std::runtime_error(message)
        {
        }
    };

#endif /* !COREEXCEPTION_HPP_ */
