/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowContext
*/

#ifndef WINDOWCONTEXT_HPP_
    #define WINDOWCONTEXT_HPP_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

class WindowContext {
public:
    enum MenuField {
        IP_FIELD = 0,
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

    struct Assets {
        sf::Music menuMusic;
        sf::Font font;
        bool fontLoaded;
        bool menuMusicLoaded;
        sf::Texture tileTexture;
        sf::Texture linemateTexture;
    };

    struct View {
        sf::View world;
        sf::Vector2f center;
        sf::Clock panClock;
        float zoomLevel;
    };

    struct Selection {
        std::optional<std::pair<int, int>> tile;
        std::optional<int> playerId;
        std::optional<std::pair<int, int>> pendingTileRequest;
        std::optional<std::string> openedTeamInventory;
    };

    struct Menu {
        std::string ip;
        std::string port;
        std::string status;
        std::vector<std::string> teamNames;
        bool connectionEdited;
        MenuField activeField;
    };

    struct Time {
        std::optional<int> pendingRequest;
        int serverFreq;
        int resumeServerFreq;
        bool paused;
    };

    struct Display {
        bool showGrid;
        bool showBroadcasts;
        bool showInfoPanel;
        bool showLegend;
    };

    WindowContext(unsigned int width, unsigned int height,
        const std::string& title);

    sf::RenderWindow window;
    Assets assets;
    View view;
    Selection selection;
    Menu menu;
    Time time;
    Display display;
    unsigned int width;
    unsigned int height;
    std::string title;
};

#endif /* !WINDOWCONTEXT_HPP_ */
