#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "game.hpp"

using SPP::Game;

int Game::disposal_period = 600;
int Game::default_m_volume = 16;
int Game::default_s_volume = 32; 
 
Game::Game(const char* title, bool fs, bool db)
: window{RenderWindow(title, WINDOW_WIDTH, WINDOW_HEIGHT, fs)}
, audio{Audio(default_m_volume, default_s_volume)}
, debug{db}, running{true}
{ 
	window.open(); // If you don't want the window to open, override this in the derived class
}

void Game::play()
{
	while (running) {	
		update();
	}
}

void Game::update()
{
	SDL_PumpEvents(); // get most recent event

	if (debug) { // for testing, to let us quit easily
		SDL_LoadEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN: 

				switch(event.key.keysym.sym)
				{ 
					case SDLK_q:
					case SDLK_ESCAPE:
						running = false;
						break; 

                	default:
                    	break;
            	}
            	
			default:
				break;
		}
	}

	for (auto& [l,v] : queue.layers) { // update the entities according to their logic in the render queue 
		for (auto e : v) {
			e->update();
		}
	}

	while (!BroadcastQueue::emptyHuh()) { // process broadcasts
		dispatch(BroadcastQueue::extract());
	}

	window.clear();
	window.render(queue); // render items from renderqueue
	window.display();

	if ((Timer::time_f() % disposal_period) == 0) {
		queue.dispose(); // emtpy dustbin every disposal_period frames
	}

	SDL_DiscardEvent(&event_void); // discard most recent event

	Timer::tick(); // increment timer
}

void Game::dispatch(Broadcast b) // defined despite being pure virtual so that
{ // these common broadcasts can be invoked by any game
	switch (b.message) {
		
		case QUERY_TIME:
			std::cout << "Time elapsed (in frames): " << Timer::time_f() << '\n';
			break;

		case SHOW_ENTITY:
			queue.display(static_cast<Entity*>(b.data_ptr), b.data_int);
			break;

		case HIDE_ENTITY:
			queue.remove(static_cast<Entity*>(b.data_ptr), false);
			break;

		case KILL_ENTITY:
			queue.remove(static_cast<Entity*>(b.data_ptr), true);
			break;

		case CHANGE_VISIBILITY_ENTITY:
			queue.changeVisibility(static_cast<Entity*>(b.data_ptr), (char)b.data_int);
			break;

		case PLAY_SOUND:
			audio.playSound(reinterpret_cast<const char*>(b.data_ptr));
			break;

		case PLAY_MUSIC:
			audio.startMusic(reinterpret_cast<const char*>(b.data_ptr));
			break;

		case SILENCE_SOUND:
			audio.stopSound();
			break;

		case SILENCE_MUSIC:
			audio.stopMusic();
			break;

		// etc...

		default:
			break;
	}
} // Processing game-specific broadcasts is delegated to the derived class