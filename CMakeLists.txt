cmake_minimum_required(VERSION 3.20)
project(Zappy LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

# ----------------------------------------------------------------======
# 1. CIBLE : zappy_server
# ----------------------------------------------------------------======

file(GLOB_RECURSE SERVER_SOURCES "server/*.cpp")

add_executable(zappy_server ${SERVER_SOURCES})
target_include_directories(zappy_server PRIVATE server/include GameEngine/include)

# ----------------------------------------------------------------======
# 2. CIBLE : zappy_gui
# ----------------------------------------------------------------======

file(GLOB_RECURSE GUI_SOURCES "GameEngine/src/*.cpp")

add_executable(zappy_gui ${GUI_SOURCES})
target_include_directories(zappy_gui PRIVATE GameEngine/include server/include)

find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)
target_link_libraries(zappy_gui PRIVATE sfml-graphics sfml-window sfml-system sfml-network)

# ----------------------------------------------------------------======
# 3. CIBLE : zappy_ai (Déploiement du script Python)
# ----------------------------------------------------------------======

add_custom_target(zappy_ai ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/IA/main.py ${CMAKE_CURRENT_BINARY_DIR}/zappy_ai
    COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/zappy_ai
    COMMENT "Déploiement de l'exécutable IA (Copie de main.py)"
)
