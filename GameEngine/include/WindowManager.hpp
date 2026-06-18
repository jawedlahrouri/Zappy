/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

#include "GameWorld.hpp"
#include "IGraphic.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class WindowManager : public IGraphic {
public:
    WindowManager(unsigned int width = 1920, unsigned int height = 1080, const std::string& title = "Zappy GUI");
    ~WindowManager() override;

    bool isOpen() const override;

    bool handleMenuEvents() override;
    bool handleGameEvents(const GameWorld& world) override;
    std::optional<std::pair<int, int>> consumeTileRequest() override;
    std::optional<int> consumeTimeRequest() override;

    void clear() override;
    void display() override;

    void drawMenu() override;
    void drawWorld(const GameWorld& world) override;
    const std::string& getMenuIp() const override;
    const std::string& getMenuPort() const override;
    void setMenuConnectionInfo(const std::string& ip,
        const std::string& port) override;
    void setMenuStatus(const std::string& status) override;
    void resetMenuTeamNames() override;
    void addMenuTeamName(const std::string& teamName) override;
    bool consumeMenuConnectionEdited() override;
    void setServerFreq(int freq) override;
    int getServerFreq() const override;
    bool isTimePaused() const override;

    private:
        static constexpr float ZOOM_STEP = 1.1f;
        static constexpr float MIN_ZOOM = 0.35f;
        static constexpr float MAX_ZOOM = 3.0f;
        static constexpr float PAN_SPEED = 650.0f;
        static constexpr int MIN_SERVER_FREQ = 1;
        static constexpr int MAX_SERVER_FREQ = 1000;

        enum MenuField {
            IP_FIELD = 0,
            PORT_FIELD = 1
        };

        void create2DWindow(unsigned int width, unsigned int height);
        void loadTextures();
        void loadFont();
        void loadMenuMusic();
        void playMenuMusic();
        void stopMenuMusic();
        void updateView(unsigned int width, unsigned int height);
        void zoomView(float factor);
        void resetZoom();
        void update2DPan();
        bool handleWindowEvent(const sf::Event& event);
        bool handleGameEvent(const sf::Event& event, const GameWorld& world);
        void handleMenuTextEntered(sf::Uint32 unicode);
        bool handleMenuMousePressed(const sf::Event& event);
        std::string& getActiveMenuInput();
        bool select2DTile(const GameWorld& world, int mouseX, int mouseY);
        bool select2DPlayer(const GameWorld& world, int mouseX, int mouseY);
        bool handleTimeControlMousePressed(int mouseX, int mouseY);
        bool handleGlobalInfoMousePressed(const GameWorld& world, int mouseX,
            int mouseY);
        bool isInsideSelectionPanel(int mouseX, int mouseY) const;
        bool isInsideTimeControlPanel(int mouseX, int mouseY) const;
        bool isInsideGlobalInfoPanel(int mouseX, int mouseY) const;
        void clearSelection();

        sf::RenderWindow _window;
        sf::Music _menuMusic;
        sf::View _worldView;
        sf::Font _font;
        sf::Vector2f _viewCenter;
        sf::Clock _panClock;
        std::optional<std::pair<int, int>> _selectedTile;
        std::optional<int> _selectedPlayerId;
        std::optional<std::pair<int, int>> _pendingTileRequest;
        std::optional<int> _pendingTimeRequest;
        std::optional<std::string> _openedTeamInventory;
        float _zoomLevel;
        unsigned int _width;
        unsigned int _height;
        std::string _title;
        bool _fontLoaded;
        bool _menuMusicLoaded;
        sf::Texture _tileTexture;
        sf::Texture _linemateTexture;
        std::string _menuIp;
        std::string _menuPort;
        std::string _menuStatus;
        std::vector<std::string> _menuTeamNames;
        bool _menuConnectionEdited;
        bool _showGrid;
        bool _showInfoPanel;
        int _serverFreq;
        int _resumeServerFreq;
        bool _timePaused;
        MenuField _activeMenuField;
    };

#endif /* !WINDOWMANAGER_HPP_ */
