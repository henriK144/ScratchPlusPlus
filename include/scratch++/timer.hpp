#pragma once
#include <SDL2/SDL.h>
namespace SPP {
class Timer
{ // A wrapper to package SDL_Timer's functions into an object.
private:
	static Uint64 underlyingTime; // The time given by SDL_GetTicks64()
	static Uint64 underlyingTime_f;
	Uint64 offset; // The offset needed to calculate the relative time for this timer
	Uint64 offset_f;
public:
	Timer(); // The non-static time functions return the amount of time since this 
	// timer object was created or since it was last reset, whichever is more recent

	static void tick(); // Updates the underlying time 

	static Uint64 time(); // Returns the underlying time
	static Uint64 time_f(); // Same but in frames

	Uint64 timeElapsed(); // Returns the time since this timer object was instantiated
	Uint64 timeElapsed_f(); // Same but in frames

	void reset(); // Resets the local time
}; // Note that the timer cannot update itself automatically; A game must call Timer::tick()
// every frame to do this.
}
