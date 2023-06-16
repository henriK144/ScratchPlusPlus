#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

#include "broadcast.hpp"
#include "audio.hpp"
#include "entity.hpp"
#include "fsm.hpp"
#include "renderqueue.hpp"
#include "renderwindow.hpp"
#include "spritesheet.hpp"
#include "timer.hpp"

#include "types/sdltypes.hpp"
#include "libraries/constants.hpp"

using std::string;
using std::map;

namespace SPP {
class Game
{ // An abstract base class that implements the basic structure of a game.
protected:
	/* Subsystems */
	RenderWindow window;
	RenderQueue queue;
	Audio audio;
	SDL_Event event;
	SDL_Event event_void;

	/* Subsystem Metadata */
	static int disposal_period;
	static int default_m_volume;
	static int default_s_volume; // these may be modified by specific games 

	/* Game Metadata */
	bool debug;
	bool running;

	/* Subclass Sandbox Stuff*/
	template<typename... Args>
	using FactoryMethod = std::function<Entity*(Args...)>; 
	// An alias for a function object to produce dynamically allocated entities
	// for the specific games. Factory methods are best implemented as lambdas because
	// they can capture components and data from the constructor scope that are not needed elsewhere.

public:
	Game(const char* title, bool fs, bool db); // title is the label on the window, fs is fullscreen, and
	// db is a debug mode that allows the game to be terminated by pressing q or esc.
	inline virtual ~Game() = 0;

	virtual void dispatch(Broadcast b); // dispatches the broadcast's logic to the program (i.e. "When I receive" in Scratch)
	virtual void update(); // The updating can have added functionality in the derived classes
	virtual void play(); // Plays the game until the player terminates it

	friend int ::main(int argc, char* args[]); // allows main to access the subsystems directly
};

Game::~Game() {}
}

// Also, here's an idea if you're up for the challenge:
// For games with a lot of dynamic entities, or ones 
// with nontrivial and expensive construction methods, 
// consider making a "FactoryCache" structure to store them. 
// In the same way that bakeries don't bake bread to order, but instead 
// stock their shelves periodically, a factory cache can call factory methods
// several times up front, storing the result, and then dole it 
// out immediately at the game's behest. There's a good chance that
// any game simple enough to be created by this engine is
// also simple enough that it would not noticeably 
// benefit from this, but it's something to think about.
