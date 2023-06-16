#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

using std::string;
namespace SPP {
namespace utils 
{ // A namespace of general-purpose helper functions (without Scratch++ dependencies) for use elsewhere
	string getName(string filepath); // extracts the filename, minus the extension, from a path to a .wav or .png file.
	string getName(const char* filepath); // (if needed, one can easily generalize this to other file types later.)
	void fixWindowsFP(string& filepath); // Thanks, windows.

	inline void* track(const char* lit) { return const_cast<void*>(reinterpret_cast<const void*>(lit)); }
	// for use with broadcasting a pointer which is a string literal
}
}
