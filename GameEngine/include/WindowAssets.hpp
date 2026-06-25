/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowAssets
*/

#ifndef WINDOWASSETS_HPP_
    #define WINDOWASSETS_HPP_

#include <filesystem>
#include <iostream>

class WindowContext;

class WindowAssets {
    public:
        WindowAssets() = delete;

        static void createWindow(WindowContext& context, unsigned int width, unsigned int height);
        static void loadTextures(WindowContext& context);
        static void loadFont(WindowContext& context);
        static void loadMusic(WindowContext& context);
        static void playMusic(WindowContext& context);
        static void stopMusic(WindowContext& context);
    };

#endif /* !WINDOWASSETS_HPP_ */
