##
## EPITECH PROJECT, 2026
## Zappy-1
## Description:
## Makefile for Zappy binaries (zappy_server, zappy_gui, zappy_ai)
##

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20

SERVER_DIR = server
SERVER_SRC = $(wildcard $(SERVER_DIR)/src/*.cpp)
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
SERVER_INC = -I$(SERVER_DIR)/include

GUI_DIR = GameEngine
GUI_SRC = $(wildcard $(GUI_DIR)/src/*.cpp)
GUI_OBJ = $(GUI_SRC:.cpp=.o)
GUI_INC = -I$(GUI_DIR)/include

AI_DIR = IA
AI_SRC = $(AI_DIR)/main.py


all: zappy_server zappy_gui zappy_ai

zappy_server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_server $(SERVER_OBJ) $(SERVER_INC)

zappy_gui: $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_gui $(GUI_OBJ) -lsfml-network -lsfml-system $(GUI_INC)

zappy_ai: $(AI_SRC)
	cp $(AI_SRC) zappy_ai
	chmod +x zappy_ai

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(dir $<)../include

clean:
	rm -f $(SERVER_OBJ) $(GUI_OBJ)

fclean: clean
	rm -f zappy_server zappy_gui zappy_ai

re: fclean all

.PHONY: all clean fclean re