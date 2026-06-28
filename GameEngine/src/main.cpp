/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** main
*/

/**
 * @file main.cpp
 * @brief Starts the Zappy graphical client.
 */

#include "Core.hpp"

/**
 * @brief Program entry point for the graphical client executable.
 * @param ac Argument count.
 * @param av Argument vector.
 * @return Process exit code.
 */
int main(int ac, char** av)
{
    try {
        Core core;

        return core.run(ac, av);
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 84;
    }
    return 0;
}
