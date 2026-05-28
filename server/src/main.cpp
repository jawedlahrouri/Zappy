/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** main.cpp
*/

#include <iostream>
#include <vector>
#include "Exception_Server.hpp"

struct ServerConfig {
    int port = 0;
    int width = 0;
    int height = 0;
    std::vector<std::string> teamNames;
    int clientsNb = 0;
    int freq = 0;
}; 

static void print_help() {
    std::cout << "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq" << std::endl;
}

ServerConfig parse_args(int argc, char** argv, ServerConfig config) {
    
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-p") {
            config.port = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "-x") {
            config.width = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "-y") {
            config.height = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "-n") {
            while (argv[i + 1] && argv[i + 1][0] != '-') {
                config.teamNames.push_back(argv[i + 1]);
                i++;
            }
        } else if (std::string(argv[i]) == "-c") {
            config.clientsNb = std::stoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "-f") {
            config.freq = std::stoi(argv[i + 1]);
        } else {
            throw Exception_Server("Argument non reconnu : " + std::string(argv[i]));
        }
    }
    if (config.port <= 0 || config.width <= 0 || config.height <= 0 || 
        config.clientsNb <= 0 || config.teamNames.empty()) {
        print_help();
    }
    return config;
}

int main (int argc, char** argv) {
    ServerConfig config;
    
    if (argc == 2 && std::string(argv[1]) == "--help") {
        print_help();
        return 0;
    }
    try {
        config = parse_args(argc, argv, config);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}