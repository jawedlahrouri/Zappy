##
## EPITECH PROJECT, 2026
## Zappy-1
## Description:
## Makefile for Zappy binaries (zappy_server, zappy_gui, zappy_ai)
##

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20
PKG_CONFIG ?= pkg-config
RAYLIB_PKG_CONFIG_PATH = $(HOME)/.local/lib/pkgconfig:$(HOME)/.local/lib64/pkgconfig:$(PKG_CONFIG_PATH)
RAYLIB_SYSTEM_LIBS = -lGL -lm -lpthread -ldl -lrt -lX11 -lXrandr -lXi -lXinerama -lXcursor -lXfixes
RAYLIB_PKG = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --exists raylib 2>/dev/null && echo 1 || echo 0)
RAYLIB_MANUAL = $(shell printf '#include <raylib.h>\nint main(void){return 0;}\n' | $(CXX) -x c++ - -lraylib $(RAYLIB_SYSTEM_LIBS) -o /tmp/zappy_raylib_check >/dev/null 2>&1 && rm -f /tmp/zappy_raylib_check && echo 1 || echo 0)

ifeq ($(RAYLIB_PKG),1)
    RAYLIB_CFLAGS = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --cflags raylib 2>/dev/null)
    RAYLIB_LIBS = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --libs raylib 2>/dev/null) $(RAYLIB_SYSTEM_LIBS)
    CXXFLAGS += -DZAPPY_HAS_RAYLIB=1 $(RAYLIB_CFLAGS)
else ifeq ($(RAYLIB_MANUAL),1)
    RAYLIB_LIBS = -lraylib $(RAYLIB_SYSTEM_LIBS)
    CXXFLAGS += -DZAPPY_HAS_RAYLIB=1
else
    RAYLIB_LIBS =
    CXXFLAGS += -DZAPPY_HAS_RAYLIB=0
endif

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
	$(CXX) $(CXXFLAGS) -o zappy_gui $(GUI_OBJ) -lsfml-network -lsfml-system -lsfml-graphics -lsfml-window $(RAYLIB_LIBS) $(GUI_INC)

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
