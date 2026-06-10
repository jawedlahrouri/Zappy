##
## EPITECH PROJECT, 2026
## Zappy-1
## Description:
## Makefile for Zappy binaries (zappy_server, zappy_gui, zappy_ai)
##

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20

INCLUDES = $(shell find . -type d -name include | sed 's/^/-I/')
CXXFLAGS += $(INCLUDES)

SERVER_DIR = server
SERVER_SRC = $(shell find $(SERVER_DIR) -name "*.cpp")
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

GUI_DIR = GameEngine
GUI_SRC = $(shell find $(GUI_DIR)/src -name "*.cpp")
GUI_OBJ = $(GUI_SRC:.cpp=.o)

AI_DIR = IA
AI_SRC = $(AI_DIR)/main.py

all: zappy_server zappy_gui zappy_ai

zappy_server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_server $(SERVER_OBJ)

zappy_gui: $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_gui $(GUI_OBJ) \
	-lsfml-network -lsfml-system -lsfml-graphics -lsfml-window

zappy_ai: $(AI_SRC)
	cp $(AI_SRC) zappy_ai
	chmod +x zappy_ai

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(GUI_OBJ)

fclean: clean
	rm -f zappy_server zappy_gui zappy_ai

re: fclean all

.PHONY: all clean fclean re
