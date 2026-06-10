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

PKG_CONFIG ?= pkg-config
COMMA := ,

RAYLIB_PKG_CONFIG_PATH = $(HOME)/.local/lib/pkgconfig:$(HOME)/.local/lib64/pkgconfig:$(PKG_CONFIG_PATH)
RAYLIB_SYSTEM_LIBS = -lGL -lm -lpthread -ldl -lrt -lX11 -lXrandr -lXi -lXinerama -lXcursor -lXfixes

RAYLIB_PKG = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --exists raylib 2>/dev/null && echo 1 || echo 0)

RAYLIB_MANUAL = $(shell printf '#include <raylib.h>\nint main(void){return 0;}\n' | \
$(CXX) -x c++ - -lraylib $(RAYLIB_SYSTEM_LIBS) -o /tmp/zappy_raylib_check >/dev/null 2>&1 && \
rm -f /tmp/zappy_raylib_check && echo 1 || echo 0)

ifeq ($(RAYLIB_PKG),1)
    RAYLIB_CFLAGS = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --cflags raylib 2>/dev/null)
    RAYLIB_LIBDIR = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --variable=libdir raylib 2>/dev/null)
    RAYLIB_RPATH = $(if $(RAYLIB_LIBDIR),-Wl$(COMMA)-rpath$(COMMA)$(RAYLIB_LIBDIR),)
    RAYLIB_LIBS = $(shell PKG_CONFIG_PATH="$(RAYLIB_PKG_CONFIG_PATH)" $(PKG_CONFIG) --libs raylib 2>/dev/null) $(RAYLIB_RPATH) $(RAYLIB_SYSTEM_LIBS)
    RAYLIB_AVAILABLE = 1
    CXXFLAGS += $(RAYLIB_CFLAGS)
else ifeq ($(RAYLIB_MANUAL),1)
    RAYLIB_LIBS = -lraylib $(RAYLIB_SYSTEM_LIBS)
    RAYLIB_AVAILABLE = 1
else
    RAYLIB_LIBS =
    RAYLIB_AVAILABLE = 0
endif

SERVER_DIR = server
SERVER_SRC = $(shell find $(SERVER_DIR) -name "*.cpp")
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

GUI_DIR = GameEngine
GUI_SRC_ALL = $(shell find $(GUI_DIR)/src -name "*.cpp")
GUI_RENDERER_SRC = $(GUI_DIR)/src/RaylibRenderer.cpp
GUI_RENDERER_NO_RAYLIB_SRC = $(GUI_DIR)/src/RaylibRendererNoRaylib.cpp
GUI_SRC_BASE = $(filter-out $(GUI_RENDERER_SRC) $(GUI_RENDERER_NO_RAYLIB_SRC),$(GUI_SRC_ALL))

ifeq ($(RAYLIB_AVAILABLE),1)
    GUI_SRC = $(GUI_SRC_BASE) $(GUI_RENDERER_SRC)
else
    GUI_SRC = $(GUI_SRC_BASE) $(GUI_RENDERER_NO_RAYLIB_SRC)
endif

GUI_OBJ = $(GUI_SRC:.cpp=.o)
GUI_RENDERER_OBJ = $(GUI_RENDERER_SRC:.cpp=.o) $(GUI_RENDERER_NO_RAYLIB_SRC:.cpp=.o)
GUI_CLEAN_OBJ = $(sort $(GUI_OBJ) $(GUI_RENDERER_OBJ))

AI_DIR = IA
AI_SRC = $(AI_DIR)/main.py

all: zappy_server zappy_gui zappy_ai

zappy_server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_server $(SERVER_OBJ)

zappy_gui: $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o zappy_gui $(GUI_OBJ) \
	-lsfml-network -lsfml-system -lsfml-graphics -lsfml-window \
	$(RAYLIB_LIBS)

zappy_ai: $(AI_SRC)
	cp $(AI_SRC) zappy_ai
	chmod +x zappy_ai

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(GUI_CLEAN_OBJ)

fclean: clean
	rm -f zappy_server zappy_gui zappy_ai

re: fclean all

.PHONY: all clean fclean re
