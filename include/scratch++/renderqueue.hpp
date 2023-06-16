#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <functional>

#include "libraries/constants.hpp"
#include "entity.hpp"
#include "scene.hpp"

using std::map;
using std::vector;
using std::string;
using std::pair;
namespace SPP {
using EntityPredicate = std::function<bool(Entity* eptr)>;

class RenderWindow;
class Game;

class RenderQueue 
{ // A set of "queues" to  facilitate rendering Entities onto the screen in the right order
private:
	map<int,vector<Entity*>> layers; // Entities on lower layers are rendered before those on higher ones
	vector<Entity*> dustbin; // Temporarily holds dynamically allocated entities which are to be deallocated

	// The layers map can contain both positive and negative integers.
	// The background(s) are on layer 0, and the visible entities are on positive layers.
	// The negative layers contain temporarily hidden entities, which are not visible but
	// still operate logically through their components; that is they still have a position,
	// transform, tag, collision detector, etc.

public:
	RenderQueue(); // Creates an empty queue with no background
	RenderQueue(Entity* bg); // Creates an empty queue with bg as the background

	RenderQueue& setBG(Entity* e); // Use e == nullptr to remove the background, leaving a white screen.
	RenderQueue& addBG(Entity* e); // Adds a background overtop of the existing one(s)

	RenderQueue& display(Entity* e, int l=1); // Adds e to the queue on layer l
	RenderQueue& remove(Entity* e, bool destroy); // Searches the queues for a match (by pointer equality) to the entity and removes it, sending it to the dustbin if destroy is true
	RenderQueue& remove(const string& t, bool destroy); // The same, but for tag equality
	RenderQueue& remove_when(EntityPredicate p, bool destroy); // A general version of the above, removing
	// those entities for which the predicate is true
	RenderQueue& clear(bool destroy); // Equivalent to calling remove(e, destroy) on every e in the queue except for the background(s)

	RenderQueue& changeVisibility(Entity* e, char c); // 'h' = hide, 's' = show, 't' = toggle
	// (do not pass a background entity, the behaviour is unspecified in that case.)

	RenderQueue& load(Scene& s); // loads a scene, which is a vector of the form
	// {BG, 0, a11, a12, ..., 0, a21, a22, ..., 0, ...} which specifies that each a1i is on layer 1,
	// each a2i is on layer 2, etc.
	// In fact, this means you can have multiple backgrounds if you want, since they can both be on layer 0.
	/* REMOVE FROM GITHUB VERSION */

	void dispose(); // Deallocates everything in the dustbin, which we take as a precondition to be heap-allocated
	// (if this is not the case, the game will almost certainly crash.)
	void debug(); // Prints the addresses of every rendered entity and the layer they're on, along with the contents of the dustbin
	
	friend class RenderWindow;
	friend class Game;
};
}
