cmake_minimum_required(VERSION 3.20)
project(Zappy LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(zappy_enable_warnings target_name)
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)
endfunction()
find_package(SFML 2.5 COMPONENTS graphics window system network audio REQUIRED)

find_package(raylib QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

# ----------------------------------------------------------------======
# ASTUCE EPITECH : Force les exécutables C++ à poper à la racine du repo
# ----------------------------------------------------------------======
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ----------------------------------------------------------------======
# 1. CIBLE : zappy_network_<os> (backend réseau dynamique)
# ----------------------------------------------------------------======
if(UNIX)
    add_library(zappy_network_linux SHARED
        network_backends/linux/Sys_Socket_Linux.cpp
        network_backends/linux/Sys_Poll_Linux.cpp
    )
    zappy_enable_warnings(zappy_network_linux)
    set_target_properties(zappy_network_linux PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set(NETWORK_BACKEND_TARGET zappy_network_linux)
elseif(WIN32)
    add_library(zappy_network_win SHARED
        network_backends/windows/Sys_Socket_Win.cpp
        network_backends/windows/Sys_Poll_Win.cpp
    )
    zappy_enable_warnings(zappy_network_win)
    set_target_properties(zappy_network_win PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(zappy_network_win PRIVATE ws2_32)
    set(NETWORK_BACKEND_TARGET zappy_network_win)
endif()

# ----------------------------------------------------------------======
# 2. CIBLE : zappy_server
# ----------------------------------------------------------------======
file(GLOB_RECURSE SERVER_SOURCES "server/*.cpp")

add_executable(zappy_server ${SERVER_SOURCES})
zappy_enable_warnings(zappy_server)

# On met tous tes chemins d'inclusion ici, et UNIQUEMENT pour le serveur (PRIVATE)
target_include_directories(zappy_server PRIVATE 
    server/include
    server/include/application/Command/AI/Parsing
    server/include/application/Command/GUI/Parsing
    server/include/domain/Client
    server/include/domain/GameEngine
    server/include/domain/Network
    server/include/infrastructure/Network
    server/include/infrastructure/Poll
    server/include/infrastructure/Socket
    server/include/presentation/Parsing
    server/include/shared
    GameEngine/include
)

# Lier avec dl (Linux) pour dlopen, ou ws2_32 (Windows) pour LoadLibrary
if(UNIX)
    target_link_libraries(zappy_server PRIVATE dl)
elseif(WIN32)
    target_link_libraries(zappy_server PRIVATE ws2_32)
endif()

# Le serveur doit être construit après le backend réseau
add_dependencies(zappy_server ${NETWORK_BACKEND_TARGET})

# ----------------------------------------------------------------======
# 3. CIBLE : zappy_gui + bibliothèques graphiques
# ----------------------------------------------------------------======
set(GAME_ENGINE_CORE_SOURCES
    GameEngine/src/main.cpp
    GameEngine/src/Core.cpp
    GameEngine/src/GameWorld.cpp
    GameEngine/src/NetworkManager.cpp
)

add_executable(zappy_gui ${GAME_ENGINE_CORE_SOURCES})
zappy_enable_warnings(zappy_gui)
target_include_directories(zappy_gui PRIVATE GameEngine/include)
target_link_libraries(zappy_gui PRIVATE
    sfml-network
    sfml-system
    ${CMAKE_DL_LIBS}
)

set(GRAPHIC_2D_SOURCES
    GameEngine/src/WindowManager.cpp
    GameEngine/src/WindowContext.cpp
    GameEngine/src/WindowAssets.cpp
    GameEngine/src/WindowEvents.cpp
    GameEngine/src/WindowMenu.cpp
    GameEngine/src/WindowRender.cpp
    GameEngine/src/WindowSelection.cpp
    GameEngine/src/WindowTime.cpp
    GameEngine/src/WindowView.cpp
    GameEngine/src/TextureManager.cpp
    GameEngine/src/InfoPanel.cpp
    GameEngine/src/Menu.cpp
    GameEngine/src/Players.cpp
    GameEngine/src/World.cpp
    GameEngine/src/GraphicalUtils.cpp
    GameEngine/src/GameWorld.cpp
    GameEngine/graphics/2d/Graphic2D.cpp
)

add_library(zappy_graphic_2d SHARED ${GRAPHIC_2D_SOURCES})
zappy_enable_warnings(zappy_graphic_2d)
target_include_directories(zappy_graphic_2d PRIVATE GameEngine/include)
target_link_libraries(zappy_graphic_2d PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

set(GRAPHIC_3D_SOURCES
    GameEngine/src/GameWorld.cpp
    GameEngine/graphics/3d/Graphic3DCamera.cpp
    GameEngine/graphics/3d/Graphic3D.cpp
    GameEngine/graphics/3d/Graphic3DInterface.cpp
    GameEngine/graphics/3d/Graphic3DMap.cpp
    GameEngine/graphics/3d/Graphic3DPlayers.cpp
    GameEngine/graphics/3d/Graphic3DRenderer.cpp
)

add_library(zappy_graphic_3d SHARED ${GRAPHIC_3D_SOURCES})
zappy_enable_warnings(zappy_graphic_3d)
target_include_directories(zappy_graphic_3d PRIVATE GameEngine/include)
target_link_libraries(zappy_graphic_3d PRIVATE
    raylib
)

add_dependencies(zappy_gui zappy_graphic_2d)

# ----------------------------------------------------------------======
# 3. CIBLE : zappy_ai (Déploiement du script Python)
# ----------------------------------------------------------------======

file(WRITE ${CMAKE_SOURCE_DIR}/zappy_ai "#!/bin/bash\nexec python3 \"\$(dirname \"\$0\")/IA/main.py\" \"\$@\"\n")
add_custom_target(zappy_ai ALL
    COMMAND chmod +x ${CMAKE_SOURCE_DIR}/zappy_ai
    COMMENT "Déploiement de l'exécutable IA (Copie de main.py à la racine)"
)
