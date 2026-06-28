/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowTime
*/

/**
 * @file WindowTime.hpp
 * @brief Declares GUI-side server-frequency controls.
 */

#ifndef WINDOWTIME_HPP_
    #define WINDOWTIME_HPP_

#include "InfoPanel.hpp"
#include "WindowContext.hpp"
#include <array>
#include <optional>

/**
 * @brief Handles pause, slower, and faster controls for server frequency.
 */
class WindowTime {
    public:
        WindowTime() = delete;

        /**
         * @brief Applies a click to the time-control panel.
         * @return true when the click was handled.
         */
        static bool handleMousePressed(WindowContext& context, int mouseX, int mouseY);

        /**
         * @brief Checks whether coordinates are inside the time-control panel.
         */
        static bool isInsidePanel(const WindowContext& context, int mouseX, int mouseY);

        /**
         * @brief Consumes the pending frequency request.
         */
        static std::optional<int> consumeRequest(WindowContext& context);

        /**
         * @brief Stores a frequency received from the server or selected by the user.
         */
        static void setFrequency(WindowContext& context, int freq);

        /**
         * @brief Gets the current GUI frequency.
         */
        static int getFrequency(const WindowContext& context);

        /**
         * @brief Checks whether the pause control is active.
         */
        static bool isPaused(const WindowContext& context);

    private:
        /** Ordered presets used by slower/faster buttons. */
        static constexpr std::array<int, 8> TIME_FREQ_PRESETS = {
            1, 10, 25, 50, 100, 200, 500, 1000
        };

        /**
         * @brief Gets the number of frequency presets.
         */
        static int getPresetCount();

        /**
         * @brief Finds the closest preset below the given frequency.
         */
        static int getSlowerFrequency(int freq);

        /**
         * @brief Finds the closest preset above the given frequency.
         */
        static int getFasterFrequency(int freq);
};

#endif /* !WINDOWTIME_HPP_ */
