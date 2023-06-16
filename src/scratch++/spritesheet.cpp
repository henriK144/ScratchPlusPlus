#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>

#include "spritesheet.hpp"
#include "libraries/utils.hpp"

using std::string;
using std::map;
using namespace SPP::utils;
using SPP::SpriteSheet;

SpriteSheet::SpriteSheet(RenderWindow* w)
: canvas{w}
{ ; }

SpriteSheet::SpriteSheet(RenderWindow* w, const char* dir) : SpriteSheet(w)
{
	load_sprites(dir);
}

SpriteSheet::~SpriteSheet()
{ // destructor frees all of the loaded texture pointers with SDL_DestroyTexture()
	for (auto spriter = sheet.begin(); spriter != sheet.end(); spriter++) {
		SDL_DestroyTexture(spriter->second.texture);
	}
}

SpriteSheet& SpriteSheet::load_sprite(const string& name, const char* filepath)
{ // use the subroutine from sprite.hpp
	Texture* txtrptr = canvas->loadTexture(filepath);
	if (!txtrptr) { // Check that the file exists
		std::cout << "Unable to load file " << filepath << ", it either doesn't exist or is not a png file" << '\n';
		return *this;
	}

	unsigned char buf[8];
    unsigned int pngwidth;
    unsigned int pngheight;

    std::ifstream in(filepath);
    in.seekg(16);
    in.read(reinterpret_cast<char*>(&buf), 8);

    pngwidth = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
    pngheight = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
    // does some wizardry to get the file's dimensions (ripped from stackoverflow)

    Sprite spr = {txtrptr, static_cast<int>(pngwidth), static_cast<int>(pngheight)};
    sheet[name] = spr;
    std::cout << "Loaded file " << filepath << " as " << name << "\n";
    
    return *this;
}

SpriteSheet& SpriteSheet::load_sprite(const char* filepath)
{
	load_sprite(getName(filepath), filepath);
	return *this;
}

SpriteSheet& SpriteSheet::load_sprites(const char* dir)
{
	const std::filesystem::path dirpath{dir}; // filesystem time
	string abspath;

	for (const auto& dir_entry : std::filesystem::directory_iterator{dirpath}) 
    { // iterate over the directory
    	abspath = dir_entry.path().string(); // form absolute path
    	fixWindowsFP(abspath); // replace \s with /s
        load_sprite(abspath.c_str()); // load it
    }	

    return *this;
}

SpriteSheet& SpriteSheet::load_from_text(const string& name, const string& text, TTF_Font* font, SDL_Colour colour)
{
	Texture* txtrptr = canvas->loadTextTexture(text, font, colour);
	if (!txtrptr) { // Check that the file exists
		std::cout << "Unable to load text \"" << text << "\", see other errors for details\n";
		return *this;
	}

	int txtwidth; 
	int txtheight;
	int got_dimensions = TTF_SizeText(font, text.c_str(), &txtwidth, &txtheight);
	if (got_dimensions < 0) {
		std::cout << "Something went wrong while getting dimensions, terminating load\n";
		return *this;
	}

	Sprite spr = {txtrptr, txtwidth, txtheight};

	sheet[name] = spr;
	std::cout << "Loaded text sprite \"" << text << "\" as " << name << "\n";
	return *this;
}

SpriteSheet& SpriteSheet::load_from_text(const string& text, TTF_Font* font, SDL_Colour colour)
{
	return load_from_text(text, text, font, colour);
}

Sprite& SpriteSheet::operator[] (const string& name) { return sheet.at(name); }
const Sprite& SpriteSheet::operator[] (const string& name) const { return sheet.at(name); }

void SpriteSheet::spriteInfo(const string& name)
{
	std::cout << "Sprite " << name << " has width " << sheet.at(name).width << " and height " << sheet.at(name).height << "\n";
} 

void SpriteSheet::aliasAs(const string& name, const string& alias) { sheet[alias] = sheet.at(name); }