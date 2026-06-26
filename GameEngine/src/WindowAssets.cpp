/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowAssets
*/

#include "WindowAssets.hpp"

void WindowAssets::createWindow(WindowContext& context, unsigned int width, unsigned int height)
{
    if (context.window.isOpen())
        context.window.close();

    context.width = width;
    context.height = height;
    context.window.create(sf::VideoMode(width, height), context.title);
    context.window.setFramerateLimit(60);
    context.window.requestFocus();
    context.view.center = {static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f};
    context.view.panClock.restart();
    WindowView::update(context, width, height);
}

void WindowAssets::loadTextures(WindowContext& context)
{
    (void)context;
    TextureManager& tm = TextureManager::getInstance();
    const auto loadSayenAnimation = [&tm](const std::string& color,
        const std::string& folder, const std::string& suffix) {
        for (int frame = 1; frame <= 17; frame++) {
            const std::string frameNumber =
                frame < 10 ? "0" + std::to_string(frame) : std::to_string(frame);

            tm.loadTexture(color + "Sayen" + frameNumber,
                folder + "/sayen" + frameNumber + suffix + ".png");
        }
    };

    tm.loadTexture("menuBackground", "background_menu.png");
    tm.loadTexture("spursBackground", "spurs_background.png");
    tm.loadTexture("ground", "ground.png");
    tm.loadTexture("food", "food.png");
    tm.loadTexture("linemate",  "brique.png");
    tm.loadTexture("deraumere", "shoe.png");
    tm.loadTexture("sibur", "ballon.png");
    tm.loadTexture("mendiane", "jersey.png");
    tm.loadTexture("phiras", "trophy.png");
    tm.loadTexture("thystame",  "carte.png");
    tm.loadTexture("egg", "oeuf.png");
    tm.loadTexture("timePauseButton", "1boutonpause.png");
    tm.loadTexture("timePlayButton", "boutonplay.png");
    tm.loadTexture("timeSlowerButton", "boutongauche.png");
    tm.loadTexture("timeFasterButton", "boutondroite.png");
    tm.loadTexture("infoButton", "button_info.png");

    tm.loadTexture("playerFace", "character_face.png");
    tm.loadTexture("playerBack", "character_back.png");
    tm.loadTexture("playerRight", "character_right.png");
    tm.loadTexture("playerLeft", "character_left.png");
    tm.loadTexture("whiteRedFace", "white_red_face.png");
    tm.loadTexture("whiteRedBack", "white_red_back.png");
    tm.loadTexture("whiteRedRight", "white_red_right.png");
    tm.loadTexture("whiteRedLeft", "white_red_left.png");

    loadSayenAnimation("red", "redsayen", "");
    loadSayenAnimation("blue", "bluesayen", "blue");
    loadSayenAnimation("green", "greensayen", "green");
    loadSayenAnimation("pink", "pinksayen", "pink");

    const std::string deathColors[] = {"red", "blue", "green", "pink"};

    for (const std::string& color : deathColors) {
        for (int frame = 0; frame < 7; frame++) {
            const std::string key = color + "Death" + std::to_string(frame);
            const std::string file = color + "_death_" +
                std::to_string(frame) + ".png";

            tm.loadTexture(key, file);
        }
    }

    tm.loadTexture("bluePlayer", "blue_character.png");
    tm.loadTexture("blueBack", "blue_back.png");
    tm.loadTexture("blueRight", "blue_right.png");
    tm.loadTexture("blueLeft", "blue_left.png");
    tm.loadTexture("whiteBlueFace", "white_blue_face.png");
    tm.loadTexture("whiteBlueBack", "white_blue_back.png");
    tm.loadTexture("whiteBlueRight", "white_blue_right.png");
    tm.loadTexture("whiteBlueLeft", "white_blue_left.png");

    tm.loadTexture("greenPlayer", "green_character.png");
    tm.loadTexture("greenBack", "green_back.png");
    tm.loadTexture("greenRight", "green_right.png");
    tm.loadTexture("greenLeft", "green_left.png");
    tm.loadTexture("whiteGreenFace", "white_green_face.png");
    tm.loadTexture("whiteGreenBack", "white_green_back.png");
    tm.loadTexture("whiteGreenRight", "white_green_right.png");
    tm.loadTexture("whiteGreenLeft", "white_green_left.png");

    tm.loadTexture("pinkPlayer", "pink_character.png");
    tm.loadTexture("pinkBack", "pink_back.png");
    tm.loadTexture("pinkRight", "pink_right.png");
    tm.loadTexture("pinkLeft", "pink_left.png");
    tm.loadTexture("whitePinkFace", "white_pink_face.png");
    tm.loadTexture("whitePinkBack", "white_pink_back.png");
    tm.loadTexture("whitePinkRight", "white_pink_right.png");
    tm.loadTexture("whitePinkLeft", "white_pink_left.png");
}

void WindowAssets::loadFont(WindowContext& context)
{
    const std::string fonts[] = {
        "GameEngine/assets/fonts/arial.ttf",
        "GameEngine/assets/fonts/NimbusSans-Regular.otf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/opentype/urw-base35/NimbusSans-Regular.otf"
    };

    for (const std::string& path : fonts) {
        if (std::filesystem::exists(path) &&
            context.assets.font.loadFromFile(path)) {
            context.assets.fontLoaded = true;
            return;
        }
    }
    std::cerr << "[WindowManager] Police introuvable: panneau d'information "
        << "2D sans texte." << std::endl;
}

void WindowAssets::loadMusic(WindowContext& context)
{
    const std::string musicPath =
        "GameEngine/assets/sprites/Chicago_Bulls_intro.mp3";

    if (!context.assets.menuMusic.openFromFile(musicPath)) {
        std::cerr << "[WindowManager] Musique de menu introuvable: "
            << musicPath << std::endl;
        return;
    }
    context.assets.menuMusic.setLoop(true);
    context.assets.menuMusicLoaded = true;
}

void WindowAssets::playMusic(WindowContext& context)
{
    if (!context.assets.menuMusicLoaded)
        return;
    if (context.assets.menuMusic.getStatus() != sf::Music::Playing)
        context.assets.menuMusic.play();
}

void WindowAssets::stopMusic(WindowContext& context)
{
    if (!context.assets.menuMusicLoaded)
        return;
    if (context.assets.menuMusic.getStatus() != sf::Music::Stopped)
        context.assets.menuMusic.stop();
}
