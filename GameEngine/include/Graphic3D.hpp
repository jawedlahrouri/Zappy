/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3D
*/

#ifndef GRAPHIC3D_HPP_
    #define GRAPHIC3D_HPP_

    #include "IGraphic.hpp"
    #include <SFML/Graphics.hpp>
    #include <optional>
    #include <string>
    #include <utility>

class Graphic3D : public IGraphic {
public:
    Graphic3D();
    ~Graphic3D() override;

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
    static constexpr float TILE_WIDTH = 64.0f;
    static constexpr float TILE_HEIGHT = 32.0f;
    static constexpr float TILE_DEPTH = 14.0f;

    void createWindow(unsigned int width, unsigned int height);
    void loadFont();
    void handleCloseEvent(const sf::Event& event, bool& exitToMenu);
    void drawIsoTile(sf::RenderTarget& target, float centerX, float centerY,
        sf::Color fillColor, sf::Color outlineColor);
    void drawSceneBackground(sf::RenderTarget& target);
    void drawIsoGrid(sf::RenderTarget& target, int width, int height,
        float originX, float originY, bool previewMode);
    void drawMenuLabel(sf::RenderTarget& target);
    sf::Color tileColorFromResources(const Tile& tile) const;

    sf::RenderWindow _window;
    sf::Font _font;
    bool _fontLoaded;
    std::string _menuIp;
    std::string _menuPort;
    std::string _menuStatus;
    bool _menuConnectionEdited;
    int _serverFreq;
    bool _timePaused;
};

#endif /* !GRAPHIC3D_HPP_ */
