#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "entity.hpp"
#include "renderqueue.hpp"
#include "libraries/utils.hpp"
#include "types/sprite.hpp"
#include "types/sdltypes.hpp"

using std::string;
namespace SPP {
class RenderWindow 
{ // The window that the game is displayed from.
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool fullscreen;

public:
	RenderWindow(const char* title, int w, int h, bool f); // w and h are the dimensions, f indicates fullscreen mode.
	~RenderWindow();
	
	Texture* loadTexture(const char* filepath); // Loads a texture to be displayed onto the window
	Texture* loadTextTexture(const string& text, TTF_Font* font, SDL_Colour colour); // Creates a texture from text
	Sprite makeTextSprite(const string& text, TTF_Font* font, SDL_Colour colour); // Creates a full sprite out of text
	
	void raw_render(Sprite s, float x, float y, float scale); // Renders a sprite at (x,y), scaled up or down as indicated
	void raw_render_transformed(Sprite s, float x, float y, float scale, const RenderTransform& t); // Same, but with transforms
	void raw_render_fullscreen(Sprite s); // Renders a sprite in fullscreen (scaling it up to 1920 x 1080 in the process)

	void render(const Entity& e); // Renders an Entity
	void renderFullscreen(const Entity& e); // Renders an Entity in fullscreen (scaling it up to 1920 x 1080 in the process)
	void render(const RenderQueue& q); // Updates the window according to the contents of q
	
	void open(); // shows the window
	void close(); // hides the window

	void display(); // Displays everything which is rendered
	void clear(); // Clears the screen
	void cleanUp(); // Frees the resources used by the window (same as destructor)

	void setFullscreen(); // Self-explanatory
	void exitFullscreen();
	bool fullscreenHuh();
}; 
}