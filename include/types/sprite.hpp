#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "types/sdltypes.hpp"
namespace SPP {
struct Sprite
{ // Packages an SDL_Texture and its dimensions.
	Texture* texture;
	int width;
	int height;
}; // (sizeof(Sprite) is 16, so it's fairly inexpensive to pass these by value, which is recommended.)

inline constexpr Sprite emptySprite = {nullptr, 0, 0}; // This sprite is not visible.

inline constexpr bool operator==(Sprite s1, Sprite s2)
{ return s1.texture == s2.texture; }

inline constexpr bool operator!=(Sprite s1, Sprite s2)
{ return s1.texture != s2.texture; }
}
