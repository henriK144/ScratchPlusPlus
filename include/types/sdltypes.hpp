#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDl2/SDL_ttf.h>
#include <iostream>
namespace SPP {
// A collection of aliases for commonly used SDL_ types.

/*
struct SDL_FPoint 
{
	float x;
	float y;
};
*/ 
using Point = SDL_FPoint; // A point on the screen
using PointInt = SDL_Point; // probably unused
/*
struct SDL_FRect
{
	float x;
	float y;
	float w;
	float h;
}
*/
using Rect = SDL_FRect; // A rectangular region on the screen
using RectInt = SDL_Rect; // we need this for rendering

using Colour = SDL_Colour;
using Texture = SDL_Texture;
using Font = TTF_Font;
using Event = SDL_Event;

inline void SDL_LoadEvent(SDL_Event* e) // Stores the most recent event into *e
{ SDL_PeepEvents(e, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT); }

inline void SDL_DiscardEvent(SDL_Event* e) //  Stores the most recent event into *e, and removes it from the event queue 
{ SDL_PeepEvents(e, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT); }
}
