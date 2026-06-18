cmake_minimum_required(VERSION 3.20)
project(ZappyGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

find_package(SFML 2.5 COMPONENTS graphics window system network audio REQUIRED)

set(GAME_ENGINE_CORE_SOURCES
    src/main.cpp
    src/Core.cpp
    src/GameWorld.cpp
    src/NetworkManager.cpp
)

add_executable(zappy_gui ${GAME_ENGINE_CORE_SOURCES})
target_include_directories(zappy_gui PRIVATE include)
target_link_libraries(zappy_gui PRIVATE
    sfml-network
    sfml-system
    ${CMAKE_DL_LIBS}
)
set_target_properties(zappy_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GRAPHIC_2D_SOURCES
    src/WindowManager.cpp
    src/TextureManager.cpp
    src/InfoPanel.cpp
    src/Menu.cpp
    src/Players.cpp
    src/World.cpp
    src/GraphicalUtils.cpp
    src/GameWorld.cpp
    graphics/2d/Graphic2D.cpp
)

add_library(zappy_graphic_2d SHARED ${GRAPHIC_2D_SOURCES})
target_include_directories(zappy_graphic_2d PRIVATE include)
target_link_libraries(zappy_graphic_2d PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)
set_target_properties(zappy_graphic_2d PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GRAPHIC_3D_SOURCES
    src/GameWorld.cpp
    graphics/3d/Graphic3D.cpp
)

add_library(zappy_graphic_3d SHARED ${GRAPHIC_3D_SOURCES})
target_include_directories(zappy_graphic_3d PRIVATE include)
target_link_libraries(zappy_graphic_3d PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
set_target_properties(zappy_graphic_3d PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
