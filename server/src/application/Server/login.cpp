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

/**
 * @brief Counts the eggs (free slots) currently available for a team.
 */
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

/**
 * @brief Places a new AI on its egg's tile with a random orientation.
 * @details Emits an @c ebo event (egg hatched) for the GUIs; falls back to
 *          (0,0) facing north if no egg is provided.
 */
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
}

/**
 * @brief Performs the AI login handshake and spawns the player.
 *
 * @par Protocol (AI protocol)
 * @verbatim
 *   <-- WELCOME            (already sent on accept)
 *   --> TEAM-NAME          (this team_name)
 *   <-- CLIENT-NUM         (remaining free slots)
 *   <-- X Y                (map dimensions)
 * @endverbatim
 * Consumes one team egg, places the player on it and emits a @c pnw event.
 * Replies "0" and fails if the team has no slot left.
 */
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

/**
 * @brief Performs the GUI login handshake and sends the full world snapshot.
 *
 * @par Protocol (GUI protocol)
 * @verbatim
 *   <-- WELCOME            (already sent on accept)
 *   --> GRAPHIC
 *   <-- msz X Y            (map size)
 *   <-- sgt T              (current time unit)
 *   <-- tna N              (one per team)
 * @endverbatim
 * Then streams the initial state: every tile (@c bct), every egg (@c enw) and
 * every connected player (@c pnw + @c plv) so the GUI can render the world.
 */
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
                    network.sendResponse(fd, "plv #" + std::to_string(ai->getId()) + " " + std::to_string(ai->getLevel()));
                }
            }
        }
    } catch (const std::exception &e) {
        throw ServerException(std::string("Login::login_gui: ") + e.what());
    }
}