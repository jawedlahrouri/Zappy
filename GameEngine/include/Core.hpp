/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#ifndef CORE_HPP_
    #define CORE_HPP_

    #include "NetworkManager.hpp"
    #include "GameWorld.hpp"
    #include "WindowManager.hpp"
    #include <sstream>
    #include <chrono>
    #include <unordered_map>
    #include <functional>

    class Core {
    public:
        Core();
        ~Core();
        int run(int ac, char** av);

    private:
        enum AppState {
            MENU,
            GAME
        };

        void setupMenuDefaults(int ac, char** av);
        bool connectFromMenu();
        void parseCommand(const std::string& command);
        void sendPendingTileRequest();
        void sendPendingTimeRequest();
        void initCommands();
        void parseValue(std::stringstream& ss, int& value);

        WindowManager _windowManager;
        NetworkManager _network;
        GameWorld _world;
        AppState _appState;
        bool _isRunning;
        std::unordered_map<std::string, std::function<void(std::stringstream&)>> _commands;
    };

#endif /* !CORE_HPP_ */
