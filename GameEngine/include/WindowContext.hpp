/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowContext
*/

/**
 * @file WindowContext.hpp
 * @brief Groups the mutable state shared by the SFML window modules.
 */

#ifndef WINDOWCONTEXT_HPP_
    #define WINDOWCONTEXT_HPP_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

/**
 * @brief Runtime state container for the SFML graphical backend.
 *
 * This class intentionally keeps data grouped by responsibility so the
 * Window* helper classes can remain stateless and operate on the same context.
 */
class WindowContext {
public:
    /**
     * @brief Editable fields available in the connection menu.
     */
    enum MenuField {
        /** IP address input field. */
        IP_FIELD = 0,
        /** Port input field. */
        PORT_FIELD = 1
    };

    static constexpr float ZOOM_STEP = 1.1f;
    static constexpr float MIN_ZOOM = 0.35f;
    static constexpr float MAX_ZOOM = 3.0f;
    static constexpr float PAN_SPEED = 650.0f;
    static constexpr int MIN_SERVER_FREQ = 1;
    static constexpr int MAX_SERVER_FREQ = 1000;
    static constexpr int PAUSE_SERVER_FREQ = 1;
    static constexpr int NORMAL_SERVER_FREQ = 100;

    /**
     * @brief Loaded SFML assets and their availability flags.
     */
    struct Assets {
        /** Looping menu music instance. */
        sf::Music menuMusic;
        /** Font used by menu and HUD text. */
        sf::Font font;
        /** true when font loading succeeded. */
        bool fontLoaded;
        /** true when menu music loading succeeded. */
        bool menuMusicLoaded;
        /** Cached ground tile texture. */
        sf::Texture tileTexture;
        /** Cached linemate texture used by legacy drawing paths. */
        sf::Texture linemateTexture;
    };

    /**
     * @brief World camera and panning state.
     */
    struct View {
        /** SFML view used for world rendering. */
        sf::View world;
        /** Current view center in world coordinates. */
        sf::Vector2f center;
        /** Clock used to make keyboard panning frame-rate independent. */
        sf::Clock panClock;
        /** Current zoom multiplier. */
        float zoomLevel;
    };

    /**
     * @brief Current selection and pending server-refresh requests.
     */
    struct Selection {
        /** Selected tile, if any. */
        std::optional<std::pair<int, int>> tile;
        /** Selected player identifier, if any. */
        std::optional<int> playerId;
        /** Tile content request waiting to be sent to the server. */
        std::optional<std::pair<int, int>> pendingTileRequest;
        /** Expanded team inventory in the global HUD, if any. */
        std::optional<std::string> openedTeamInventory;
    };

    /**
     * @brief Connection menu state.
     */
    struct Menu {
        /** IP field value. */
        std::string ip;
        /** Port field value. */
        std::string port;
        /** Status text shown to the user. */
        std::string status;
        /** Team names shown before entering the world. */
        std::vector<std::string> teamNames;
        /** true when IP or port was edited since the last consume. */
        bool connectionEdited;
        /** Currently focused input field. */
        MenuField activeField;
    };

    /**
     * @brief Time-control state mirrored between the GUI and server.
     */
    struct Time {
        /** Pending frequency request selected by the user. */
        std::optional<int> pendingRequest;
        /** Latest active server frequency. */
        int serverFreq;
        /** Frequency to restore after leaving pause. */
        int resumeServerFreq;
        /** true when the pause control is active. */
        bool paused;
    };

    /**
     * @brief Visibility toggles for optional world overlays.
     */
    struct Display {
        /** Draws tile grid lines when true. */
        bool showGrid;
        /** Draws player broadcast waves when true. */
        bool showBroadcasts;
        /** Draws the global information panel when true. */
        bool showInfoPanel;
        /** Expands the legend panel when true. */
        bool showLegend;
    };

    /**
     * @brief Creates a ready-to-use rendering context.
     * @param width Initial window width in pixels.
     * @param height Initial window height in pixels.
     * @param title Window title.
     */
    WindowContext(unsigned int width, unsigned int height,
        const std::string& title);

    /** Main SFML render window. */
    sf::RenderWindow window;
    /** Loaded assets and related status flags. */
    Assets assets;
    /** World camera state. */
    View view;
    /** Current selections and pending selection requests. */
    Selection selection;
    /** Connection-menu state. */
    Menu menu;
    /** Server-frequency control state. */
    Time time;
    /** Overlay visibility flags. */
    Display display;
    /** Current window width in pixels. */
    unsigned int width;
    /** Current window height in pixels. */
    unsigned int height;
    /** Window title. */
    std::string title;
};

#endif /* !WINDOWCONTEXT_HPP_ */
