/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

#ifndef CORE_HPP_
    #define CORE_HPP_

    #include "GraphicLibrary.hpp"
    #include "NetworkManager.hpp"
    #include "GameWorld.hpp"
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
        std::string getGraphicLibraryPath(int ac, char** av) const;
        bool loadGraphicLibrary(const std::string& path);
        void parseCommand(const std::string& command);
        void sendPendingTileRequest();
        void sendPendingTimeRequest();
        void initCommands();
        void parseValue(std::stringstream& ss, int& value);
        IGraphic& graphics();

        GraphicLibrary _graphicLibrary;
        NetworkManager _network;
        GameWorld _world;
        AppState _appState;
        bool _isRunning;
        std::unordered_map<std::string, std::function<void(std::stringstream&)>> _commands;
    };

#endif /* !CORE_HPP_ */
