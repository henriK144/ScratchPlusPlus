#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h> 
#include <string>
#include <map>

#include "types/sprite.hpp"
#include "renderwindow.hpp"

using std::string;
using std::map;
namespace SPP {
class SpriteSheet 
{ // A container class providing named access to Sprites.
private:
	map<string, Sprite> sheet; // a mapping from identifiers to Sprites
	RenderWindow* canvas; // the window that they will be rendered onto

public:
	SpriteSheet(RenderWindow* w); // Constructor initializes map and render window
	SpriteSheet(RenderWindow* w, const char* dir); // Loads sprites from dir
	~SpriteSheet(); // Destructor frees all of the loaded texture pointers with SDL_DestroyTexture()
	// (but, actually, this might already be done by ~RenderWindow(), which gets invoked by ~Game()...)

	SpriteSheet& load_sprite(const string& name, const char* filepath); // Adds a sprite to the table with a specfic name
	SpriteSheet& load_sprite(const char* filepath); // Infers the name from the filepath
	SpriteSheet& load_sprites(const char* dir); // Iteratively add sprites in directory, with the key being the filename
	// (that is, "[...]/camera3.png" is stored with the string "camera3")
	SpriteSheet& load_from_text(const string& name, const string& text, TTF_Font* font, SDL_Colour colour);
	// Loads the given text as a sprite
	SpriteSheet& load_from_text(const string& text, TTF_Font* font, SDL_Colour colour); // Names the text as itself
	Sprite& operator[] (const string& name); // Gets a sprite from the table
	const Sprite& operator[] (const string& name) const; // Subscript operators often come in pairs

	void spriteInfo(const string& name); // Displays the dimensions of the sprite
	void aliasAs(const string& name, const string& alias); // Adds alias as an alternate identifier for name
};
}
