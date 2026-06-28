/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Core
*/

/**
 * @file Core.hpp
 * @brief Declares the GUI application controller.
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
    #include <set>
    #include <utility>
    #include <vector>

    /**
     * @brief Coordinates networking, protocol parsing, and graphical rendering.
     *
     * Core owns the application loop. It keeps the menu and world states in
     * sync with the Zappy server and forwards user requests to the network.
     */
    class Core {
    public:
        /**
         * @brief Builds a controller with an empty world and unloaded graphics.
         */
        Core();

        /**
         * @brief Releases owned runtime resources.
         */
        ~Core();

        /**
         * @brief Starts the graphical client.
         * @param ac Argument count received by main().
         * @param av Argument vector received by main().
         * @return Process exit code.
         * @throws CoreException when the client cannot start.
         */
        int run(int ac, char** av);

    private:
        /**
         * @brief High-level application mode.
         */
        enum AppState {
            /** Connection menu is displayed. */
            MENU,
            /** World renderer is displayed. */
            GAME
        };
        
        static constexpr int MAX_NETWORK_COMMANDS_PER_FRAME = 512;

        /**
         * @brief Checks whether an argument selects the graphical backend.
         */
        static bool isGraphicPathOption(const std::string& arg);

        /**
         * @brief Parses optional player ids from an incantation command tail.
         */
        static std::vector<int> parseTrailingPlayerIds(std::stringstream& ss);

        /**
         * @brief Initializes menu connection fields from CLI defaults.
         * @param ac Argument count.
         * @param av Argument vector.
         */
        void setupMenuDefaults(int ac, char** av);

        /**
         * @brief Opens a server connection using menu values.
         * @return true when the GUI handshake completed.
         */
        bool connectFromMenu();

        /**
         * @brief Resets the state used while loading initial menu data.
         */
        void resetMenuLoadState();

        /**
         * @brief Refreshes whether the menu has enough data to enter the world.
         */
        void updateMenuReadyStatus();

        /**
         * @brief Records that an initial tile content message was received.
         * @param x Tile X coordinate.
         * @param y Tile Y coordinate.
         */
        void trackInitialTileLoaded(int x, int y);

        /**
         * @brief Resolves the graphical shared-library path from CLI arguments.
         * @param ac Argument count.
         * @param av Argument vector.
         * @return Path to the graphical backend to load.
         */
        std::string getGraphicLibraryPath(int ac, char** av) const;

        /**
         * @brief Loads the graphical backend.
         * @param path Shared-library path.
         * @throws CoreException when the backend cannot be loaded.
         */
        void loadGraphicLibrary(const std::string& path);

        /**
         * @brief Parses one protocol line received from the server.
         * @param command Raw protocol command without framing.
         */
        void parseCommand(const std::string& command);

        /**
         * @brief Sends a pending tile-content request, if the GUI selected one.
         */
        void sendPendingTileRequest();

        /**
         * @brief Sends a pending server-frequency request, if the GUI selected one.
         */
        void sendPendingTimeRequest();

        /**
         * @brief Registers handlers for all supported graphical protocol commands.
         */
        void initCommands();

        /**
         * @brief Extracts an integer value from a command stream.
         * @param ss Stream positioned on the expected value.
         * @param value Destination updated when extraction succeeds.
         */
        void parseValue(std::stringstream& ss, int& value);

        /**
         * @brief Returns the currently loaded graphical backend.
         * @return Active graphical interface.
         */
        IGraphic& graphics();

        GraphicLibrary _graphicLibrary;
        NetworkManager _network;
        GameWorld _world;
        AppState _appState;
        bool _isRunning;
        bool _menuReady;
        bool _hasMenuTeams;
        int _expectedInitialTiles;
        std::set<std::pair<int, int>> _loadedInitialTiles;
        std::unordered_map<std::string, std::function<void(std::stringstream&)>> _commands;
    };

#endif /* !CORE_HPP_ */
