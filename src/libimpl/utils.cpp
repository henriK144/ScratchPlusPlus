#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "libraries/utils.hpp"

using namespace SPP;

string utils::getName(string filepath)
{ 
	for (int i = 1; i <= 4; ++i) { // 4 is the number of characters in ".wav" and ".png"
		filepath.pop_back();
	} 

	filepath = filepath.substr(filepath.find_last_of('/') + 1);
	return filepath;
}

string utils::getName(const char* filepath) // An overload for C-style string filepaths.
{
	return utils::getName(string(filepath));
}

void utils::fixWindowsFP(string& filepath)
{
	std::replace(filepath.begin(), filepath.end(), '\\', '/');
}