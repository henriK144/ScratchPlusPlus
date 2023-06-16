#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "types/sdltypes.hpp"

// Constants for general use.
namespace SPP {
static constexpr int FULLSCREEN_WIDTH = 1920; // The dimensions of the game's window/screen
static constexpr int FULLSCREEN_HEIGHT = 1080;
static constexpr int WINDOW_WIDTH = FULLSCREEN_WIDTH - 500;
static constexpr int WINDOW_HEIGHT = FULLSCREEN_HEIGHT - 500;

static constexpr Point CENTER_WINDOWED = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
static constexpr Point CENTER_FULLSCREEN = {FULLSCREEN_WIDTH / 2, FULLSCREEN_HEIGHT / 2};
static constexpr Point OFFSCREEN = {-9999, -9999}; // Guaranteed to be off the screen
static constexpr Rect SCREEN_WINDOWED = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
static constexpr Rect SCREEN_FULL = {0, 0, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT};

static constexpr int MIDDLEGROUND = 1;
static constexpr int BACKGROUNDOBJ = 2; 
static constexpr int OBJ = 3; 
static constexpr int ACTOR = 4; 
static constexpr int PARTICLE = 5; 
static constexpr int INFO_BOX = 6;
static constexpr int INFO_TEXT = 7; // These can be used for layer specification with RenderWindow 

static constexpr Colour WHITE = {0xFF, 0xFF, 0xFF}; // Colours which can be used to create a font
static constexpr Colour BLACK = {0, 0, 0};
static constexpr Colour RED = {0xFF, 0, 0};
static constexpr Colour GREEN = {0, 0xFF, 0};
static constexpr Colour BLUE = {0, 0, 0xFF};
static constexpr Colour CYAN = {0, 0xFF, 0xFF};
static constexpr Colour MAGENTA = {0xFF, 0, 0xFF};
static constexpr Colour YELLOW = {0xFF, 0xFF, 0};
}
