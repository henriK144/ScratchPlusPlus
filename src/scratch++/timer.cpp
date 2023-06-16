#include <SDL2/SDL.h>

#include "timer.hpp"

using SPP::Timer;

Uint64 Timer::underlyingTime = 0;
Uint64 Timer::underlyingTime_f = 0;

Timer::Timer()
{
	offset = underlyingTime;
	offset_f = underlyingTime_f;
}

void Timer::tick() { underlyingTime = SDL_GetTicks64(); ++underlyingTime_f; }
Uint64 Timer::time() { return underlyingTime; }
Uint64 Timer::time_f() { return underlyingTime_f; }
Uint64 Timer::timeElapsed() {return underlyingTime - offset; }
Uint64 Timer::timeElapsed_f() { return underlyingTime_f - offset_f; }
void Timer::reset() { offset = underlyingTime; offset_f = underlyingTime_f; } 