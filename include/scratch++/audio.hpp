#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

using std::string;
using std::map;
namespace SPP {
class Audio 
{ // A container for audio tracks which wraps the audio playback functionality of SDL_Mixer.
  // (Only wav files are permitted as of yet.)
private:
	map<string, Mix_Music*> mmap; // Background music tracks
	map<string, Mix_Chunk*> smap; // Sound effects or short melodies

	int mvolume; // music volume
	int svolume; // sound volume
	int cachedvolume; 
	bool mmuted;
	bool smuted; 
	// If the audio is muted then noises are suppressed, 
	// which is to say that the "real volume" is ([ms]muted ? 0 : [ms]volume).

public:
	Audio(int mvol, int svol); // Takes the initial volumes for music and sound, which must be between 0 and 128 inclusive
	~Audio(); // Frees all of the loaded Mix_ pointers

	// Anywhere that char audioType is an argument, 'm' is used to specify music and 's' is used to specify sound.
	// Other characters produced unspecified behaviour (but will probably be treated as 's').

	void load_track(const char* filepath); // Loads the music track at filepath
	void load_sound(const char* filepath); // Loads the sound effect at filepath
	void load_from_directory(char audioType, const char* dir); // Loads all of the files in the directory dir
	// (The keys are the file's name, so "[...]/world4.wav" is stored with the string "world4")
	void aliasAs(char audioType, const string& name, const string& alias); // Gives an alias to an existing sound or music

	void init(); // "Refreshes" the audio functionality
	void playSound(const string& sound); // Plays a sound
	void stopSound(); // Stops all sounds
	void startMusic(const string& track); // Starts the music (and loops it indefinitely)
	void toggleMusic(); // Pauses or resumes the music
	void stopMusic(); // Stops the music
	void fadeIn(const string& track, float duration=2.0); // Fades music in over the specified number of seconds
	void fadeOut(float duration=2.0); // Fades music out over the specified number of seconds

	void changeVolume(char audioType, int a); // Changes the volume

	void toggle_mmute();
	void toggle_smute(); // Toggles the mute on and off
};
}
