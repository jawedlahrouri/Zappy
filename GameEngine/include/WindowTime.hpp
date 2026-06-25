/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowTime
*/

#ifndef WINDOWTIME_HPP_
    #define WINDOWTIME_HPP_

class WindowContext;

class WindowTime {
    public:
        WindowTime() = delete;

        static bool handleMousePressed(WindowContext& context, int mouseX, int mouseY);
        static bool isInsidePanel(const WindowContext& context, int mouseX, int mouseY);
        static std::optional<int> consumeRequest(WindowContext& context);
        static void setFrequency(WindowContext& context, int freq);
        static int getFrequency(const WindowContext& context);
        static bool isPaused(const WindowContext& context);

    private:
        static constexpr std::array<int, 8> TIME_FREQ_PRESETS = {
            1, 10, 25, 50, 100, 200, 500, 1000
        };

        static int getPresetCount();
        static int getSlowerFrequency(int freq);
        static int getFasterFrequency(int freq);
};

#endif /* !WINDOWTIME_HPP_ */
