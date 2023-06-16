#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "renderwindow.hpp"

using SPP::RenderWindow;

RenderWindow::RenderWindow(const char* title, int w, int h, bool f)
: window{SDL_CreateWindow(
	title, 
	SDL_WINDOWPOS_UNDEFINED, 
	SDL_WINDOWPOS_UNDEFINED, 
	w, 
	h, 
	SDL_WINDOW_HIDDEN)}

, renderer{SDL_CreateRenderer(
	window, 
	-1, 
	SDL_RENDERER_PRESENTVSYNC)}
{
	if (!window || !renderer)
		std::cout << "Window display failed. Error: " << SDL_GetError() << '\n';

	SDL_RenderSetLogicalSize(renderer, 1920, 1080);

	if (f) {
		setFullscreen();
	} else {
		exitFullscreen();
	}
}

RenderWindow::~RenderWindow()
{
	SDL_DestroyWindow(window);
}

Texture* RenderWindow::loadTexture(const char* filepath) 
{
	Texture* texture = IMG_LoadTexture(renderer, filepath);

	if (!texture)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << '\n';

	return texture;
}

Texture* RenderWindow::loadTextTexture(const string& text, Font* font, Colour colour)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), colour);

	if (!surface)
    	std::cout << "Failed to render text. Error: " << TTF_GetError() << '\n';

	return SDL_CreateTextureFromSurface(renderer, surface);
}

Sprite RenderWindow::makeTextSprite(const string& text, TTF_Font* font, SDL_Colour colour)
{
	Texture* txtr = loadTextTexture(text, font, colour);

	int txtwidth; 
	int txtheight;
	int got_dimensions = TTF_SizeText(font, text.c_str(), &txtwidth, &txtheight);

	if (got_dimensions < 0) {
		std::cout << "Something went wrong while getting dimensions, returning an empty sprite\n";
		return emptySprite;
	}

	return {txtr, txtwidth, txtheight};
}

void RenderWindow::raw_render(Sprite s, float x, float y, float scale)
{
	RectInt src = {0, 0, s.width, s.height};
	Rect dst = {x, y, src.w * scale, src.h * scale};
	SDL_RenderCopyF(renderer, s.texture, &src, &dst);	
}

void RenderWindow::raw_render_transformed(Sprite s, float x, float y, float scale, const RenderTransform& t)
{
	RectInt src = {0, 0, s.width, s.height};
	Rect dst = {x, y, src.w * scale, src.h * scale};

	double angle = static_cast<double>(t.tilt);

	SDL_RendererFlip flip_flags = (
		(t.hInv && t.vInv) ? 
		static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL) : 
		t.hInv ?
		static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL) :
		t.vInv ?
		static_cast<SDL_RendererFlip>(SDL_FLIP_VERTICAL) :
		// default 
		static_cast<SDL_RendererFlip>(SDL_FLIP_NONE)
	);

	if (t.mods != emptyMods) { // alter texture mods according to e.mods if nontrivial, then revert
		SDL_SetTextureColorMod(s.texture, t.mods.r, t.mods.g, t.mods.b);
		SDL_SetTextureAlphaMod(s.texture, t.mods.alpha);
		SDL_RenderCopyExF(renderer, s.texture, &src, &dst, angle, NULL, flip_flags);
		SDL_SetTextureColorMod(s.texture, 0xFF, 0xFF, 0xFF);
		SDL_SetTextureAlphaMod(s.texture, 0xFF);
	} else { // otherwise, just render
		SDL_RenderCopyExF(renderer, s.texture, &src, &dst, angle, NULL, flip_flags); 
	}
}

void RenderWindow::raw_render_fullscreen(Sprite s)
{
	SDL_RenderCopyF(renderer, s.texture, NULL, NULL);
}

void RenderWindow::render(const Entity& e) 
{
	if (e.getTransform().has_value()) {
		raw_render_transformed(e.getCostume(), e.getX(), e.getY(), e.getSize(), *e.getTransform());
	} else {
		raw_render(e.getCostume(), e.getX(), e.getY(), e.getSize());
	}
}

void RenderWindow::renderFullscreen(const Entity& e)
{
	raw_render_fullscreen(e.getCostume());
}

void RenderWindow::render(const RenderQueue& q)
{ // note that the caller is responsible for invoking clear() and display()
	for (auto& [l,v] : q.layers) {
		if (l > 0) { // shown case
			for (auto e : v) {
				render(*e);
			}
		} else if (l == 0) { // background case
			for (auto e : v) {
				renderFullscreen(*e);
			}
		} else { // hidden case, don't bother rendering
			;
		}
	}
}

void RenderWindow::open() { SDL_ShowWindow(window); }
void RenderWindow::close() { SDL_HideWindow(window); }

void RenderWindow::display() { SDL_RenderPresent(renderer); }
void RenderWindow::clear() { SDL_RenderClear(renderer); }

void RenderWindow::cleanUp() { SDL_DestroyWindow(window); }

void RenderWindow::setFullscreen() 
{ 
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); 
	fullscreen = true;
}

void RenderWindow::exitFullscreen() 
{ 
	SDL_SetWindowFullscreen(window, 0); 
	fullscreen = false; 
}

bool RenderWindow::fullscreenHuh() { return fullscreen; }