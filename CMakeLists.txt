cmake_minimum_required(VERSION 3.20)
project(Zappy LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

# ----------------------------------------------------------------======
# ASTUCE EPITECH : Force les exécutables C++ à poper à la racine du repo
# ----------------------------------------------------------------======
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ----------------------------------------------------------------======
# 1. CIBLE : zappy_server
# ----------------------------------------------------------------======
file(GLOB_RECURSE SERVER_SOURCES "server/*.cpp")

add_executable(zappy_server ${SERVER_SOURCES})

# On met tous tes chemins d'inclusion ici, et UNIQUEMENT pour le serveur (PRIVATE)
target_include_directories(zappy_server PRIVATE 
    server/include
    server/include/application/Command/AI/Parsing
    server/include/application/Command/GUI/Parsing
    server/include/domain/Client
    server/include/domain/GameEngine
    server/include/domain/Network
    server/include/infrastructure/Poll
    server/include/infrastructure/Socket
    server/include/presentation/Parsing
    server/include/shared
    GameEngine/include
)


# ----------------------------------------------------------------======
# 2. CIBLE : zappy_gui
# ----------------------------------------------------------------======
file(GLOB_RECURSE GUI_SOURCES "GameEngine/src/*.cpp")

add_executable(zappy_gui ${GUI_SOURCES})
target_include_directories(zappy_gui PRIVATE GameEngine/include server/include)

find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)
# Utilisation des cibles CMake officielles de SFML
target_link_libraries(zappy_gui PRIVATE sfml-graphics sfml-window sfml-system sfml-network)

# ----------------------------------------------------------------======
# 3. CIBLE : zappy_ai (Déploiement du script Python)
# ----------------------------------------------------------------======
# On s'assure que le script Python est copié à la RACINE du repo
add_custom_target(zappy_ai ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/IA/main.py ${CMAKE_SOURCE_DIR}/zappy_ai
    COMMAND chmod +x ${CMAKE_SOURCE_DIR}/zappy_ai
    COMMENT "Déploiement de l'exécutable IA (Copie de main.py à la racine)"
)