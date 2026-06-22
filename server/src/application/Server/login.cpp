/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** login.cpp
*/

#include "ServerException.hpp"
#include "application/Server/Server.hpp"
#include "application/Server/login.hpp"
#include "GameEngine.hpp"
#include "application/Event/GUI/EventVisitor.hpp"
#include "application/Event/GUI/EnwEvent.hpp"
#include "application/Event/GUI/PnwEvent.hpp"
#include "application/Event/GUI/EboEvent.hpp"
#include "application/Command/GUI/BctCommand.hpp"
#include "application/Command/GUI/MszCommand.hpp"
#include "application/Command/GUI/SgtCommand.hpp"
#include "application/Command/GUI/TnaCommand.hpp"
#include "application/Command/GUI/GuiVisitor.hpp"
#include "AI.hpp"
#include "GUI.hpp"
#include "domain/Map/Map.hpp"

#include <random>

static int countTeamEggs(INetworkProvider &network, const std::string &team_name) {
    if (!network.getGameEngine())
        return 0;
    int slots = 0;
    auto& map = network.getGameEngine()->getMap();
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            for (const auto &e : map.getTile(x, y).getEggs()) {
                if (e->teamName == team_name) {
                    slots++;
                }
            }
        }
    }
    return slots;
}

static void spawnAiOnEgg(std::shared_ptr<AI> ai, std::shared_ptr<Egg> egg, INetworkProvider &network) {
    int x = 0;
    int y = 0;
    if (egg) {
        x = egg->x;
        y = egg->y;
        ai->setX(x);
        ai->setY(y);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 4);
        ai->setOrientation(dis(gen));
        
        EboEvent eboEvent(egg->getId());
        EventVisitor eventVisitor(network);
        eboEvent.Accept(&eventVisitor);
    } else {
        ai->setX(0);
        ai->setY(0);
        ai->setOrientation(1);
    }
    auto& engine = network.getGameEngine();
    if (engine) {
        auto& tile = engine->getMap().getTile(x, y);
        for (int i = 1; i < RESOURCE_COUNT; ++i) {
            tile.resources[i] = 0;
        }
    }
}

// protocol:
// <-- WELCOME (sent on connection accept)
// --> TEAM-NAME
// <-- CLIENT-NUM (unused slots)
// <-- X Y (map dimensions)
bool Login::login_ai(std::shared_ptr<AI> ai, const std::string &team_name, INetworkProvider &network)
{
    try {
        int slots = countTeamEggs(network, team_name);
        if (slots <= 0) {
            int fd = ai->getId();
            network.sendResponse(fd, "0");
            return false;
        }

        std::shared_ptr<Egg> egg = nullptr;
        if (network.getGameEngine()) {
            egg = network.getGameEngine()->getMap().getAndRemoveRandomEggForTeam(team_name);
        }
        if (!egg) {
            return false;
        }

        int fd = ai->getId();
        network.sendResponse(fd, std::to_string(slots - 1));

        if (network.getGameEngine()) {
            spawnAiOnEgg(ai, egg, network);
            network.sendResponse(fd, std::to_string(network.getGameEngine()->getWidth()) + " " + std::to_string(network.getGameEngine()->getHeight()));
            network.getGameEngine()->getMap().addAiToTile(ai->getId(), ai->getX(), ai->getY());
        } else {
            network.sendResponse(fd, "0 0");
        }
        
        network.sendEvent(PnwEvent(ai));
        return true;
    } catch (const std::exception &e) {
        throw ServerException(std::string("Login::login_ai: ") + e.what());
    }
}

// protocol:
// <-- WELCOME (sent on connection accept)
// --> GRAPHIC
// <-- msz X Y\n
// <-- sgt T\n
// <-- tna N\n * nbr_teams
void Login::login_gui(std::shared_ptr<GUI> gui, INetworkProvider &network)
{
    try {
        int fd = gui->getId();
        GuiVisitor guiVisitor(network);
        EventVisitor eventVisitor(network);
        eventVisitor.setTargetClient(fd);

        MszCommand mszCmd;
        mszCmd.setClient(gui);
        mszCmd.Accept(&guiVisitor);

        SgtCommand sgtCmd;
        sgtCmd.setClient(gui);
        sgtCmd.Accept(&guiVisitor);

        TnaCommand tnaCmd;
        tnaCmd.setClient(gui);
        tnaCmd.Accept(&guiVisitor);

        if (network.getGameEngine()) {
            auto& map = network.getGameEngine()->getMap();
            int width = map.getWidth();
            int height = map.getHeight();
            
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    BctCommand bctCmd;
                    bctCmd.setX(x);
                    bctCmd.setY(y);
                    bctCmd.setClient(gui);
                    bctCmd.Accept(&guiVisitor);
                }
            }

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    const Tile& tile = map.getTile(x, y);
                    for (const auto& egg : tile.getEggs()) {
                        EnwEvent enwEvent(egg->getId(), egg->getOwnerId(), x, y);
                        enwEvent.Accept(&eventVisitor);
                    }
                }
            }
        }

        for (const auto &[client_fd, client] : network.getClients()) {
            if (client->getType() == ClientType::AI) {
                auto ai = std::dynamic_pointer_cast<AI>(client);
                if (ai) {
                    PnwEvent pnwEvent(ai);
                    pnwEvent.Accept(&eventVisitor);
                }
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Login::login_gui: ") + e.what());
    }
}