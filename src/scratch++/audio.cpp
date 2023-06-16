#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>

#include "audio.hpp"
#include "libraries/utils.hpp"

using std::string;
using std::map;
using namespace SPP::utils;
using SPP::Audio;

Audio::Audio(int mvol, int svol)
:mvolume{mvol}, svolume{svol}, mmuted{false}, smuted{false}
{ ; }

Audio::~Audio()
{
	map<string, Mix_Music*>::iterator miter;
	for (miter = mmap.begin(); miter != mmap.end(); miter++) {
		Mix_FreeMusic(miter->second);
	} // destructor frees all of the loaded mix pointers

	map<string, Mix_Chunk*>::iterator siter;
	for (siter = smap.begin(); siter != smap.end(); siter++) {
		Mix_FreeChunk(siter->second);
	}
}

void Audio::load_track(const char* filepath)
{
	Mix_Music* mptr = Mix_LoadMUS(filepath);
	string name = getName(filepath);

	if (mptr) {
		mmap[name] = Mix_LoadMUS(filepath);
		std::cout << "Loaded file " << filepath << " as " << name << "\n"; // this helps for debugging
	} else {
		std::cout << "Unable to load file " << filepath << ", it either doesn't exist or is not a .wav file\n";
	}
}

void Audio::load_sound(const char* filepath)
{
	Mix_Chunk* sptr = Mix_LoadWAV(filepath);
	string name = getName(filepath);

	if (sptr) {
		smap[name] = Mix_LoadWAV(filepath);
		std::cout << "Loaded file " << filepath << " as " << name << "\n";
	} else {
		std::cout << "Unable to load file " << filepath << ", it either doesn't exist or is not a .wav file\n";
	}
}

void Audio::load_from_directory(char audioType, const char* dir)
{
	const std::filesystem::path dirpath{dir}; // filesystem time
	string abspath;

	for (const auto& dir_entry : std::filesystem::directory_iterator{dirpath}) 
    { // iterate over the directory
    	abspath = dir_entry.path().string(); // form absolute path using res/sfx/
    	fixWindowsFP(abspath); // replace \s with /s
        if (audioType == 'm') {
        	load_track(abspath.c_str()); // load it
        } else if (audioType == 's') {
        	load_sound(abspath.c_str());
        } else {
        	std::cout << "Invalid audio type, loading stopped" << "\n";
        	break;
        }
    }	
}

void Audio::init()
{
	Mix_Volume(-1,svolume); 
	Mix_VolumeMusic(mvolume);
	std::cout << "Initialized Audio" << "\n";
}

void Audio::playSound(const string& sound)
{
	if (!smuted) Mix_PlayChannel(-1, smap.at(sound), 0);
}

void Audio::stopSound()
{
	Mix_HaltChannel(-1);
}

void Audio::startMusic(const string& track)
{
	Mix_PlayMusic(mmap.at(track), -1);
}

void Audio::toggleMusic()
{
	if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

void Audio::stopMusic()
{
	Mix_HaltMusic();
}

void Audio::fadeIn(const string& track, float duration)
{
	int duration_ms = static_cast<int>(1000 * duration);
	Mix_FadeInMusic(mmap.at(track), -1, duration_ms);
}

void Audio::fadeOut(float duration)
{
	int duration_ms = static_cast<int>(1000 * duration);
	Mix_FadeOutMusic(duration_ms);
}

void Audio::changeVolume(char audioType, int a)
{
	int* volptr = (audioType == 'm' ? &mvolume : &svolume);
	*volptr += a;
	if (*volptr > 128) {
		*volptr = 128; 
		std::cout << "Maximum volume exceeded; set to 128" << "\n";
	} else if (*volptr < 0) {
		*volptr = 0;
		std::cout << "Minimum volume subceeded; set to 0" << "\n";
	}
	init();
}

void Audio::toggle_smute()
{
	smuted = !smuted;
}

void Audio::toggle_mmute()
{
	if (mmuted) {
		mmuted = false;
		mvolume = cachedvolume;
		init(); // unmute and retrieve the previous volume
	} else {
		mmuted = true;
		cachedvolume = mvolume;
		changeVolume('m',-128); // mute and store the current volume
	}
}

void Audio::aliasAs(char audioType, const string& name, const string& alias)
{
	if (audioType == 'm') {
		mmap[alias] = mmap.at(name);
	} else {
		smap[alias] = smap.at(name);
	}
}