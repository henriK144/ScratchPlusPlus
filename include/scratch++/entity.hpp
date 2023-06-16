#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <optional>
#include <memory>

#include "types/sdltypes.hpp"
#include "types/sprite.hpp"
#include "types/rendertransform.hpp"
#include "libraries/constants.hpp"
#include "broadcast.hpp"
#include "components.hpp"
#include "geometry.hpp"

using std::vector;
using std::optional;
using std::string;
using std::unique_ptr;
using namespace std::string_literals;

namespace SPP {
using geometry::Corner;

class RenderQueue;
class RenderWindow; // These are just forward declarations for the friends

class Entity
{ // An object representing any visible thing in the game together with its internal state.
private:
	float x; // The position of the entity on the screen. 
	float y; // (0,0) is the top-left corner, (FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT) is the bottom-right.
	float size; // The size of an entity relative to the sprite it sources from. (e.g. A size of 1.5 means 1.5x larger)

	Sprite currentSprite; // The sprite that the entity is using
	vector<Sprite> costumes; // A list of other sprites which can be switched to
	int costume_index; // The index currently active costume in costumes, or -1 if costumes is unused
	// (This is not the case unless the entity expressly changes costumes to one not in the costumes vector)

	vector<unique_ptr<Component::ComponentType>> components; // A list of components attached to the Entity (see components.hpp)
	void* gameData; // A pointer to arbirary an game object or data that this entity represents or contains

	optional<RenderTransform> transform; // A visual transformation of the entity
	string tag; // For identification purposes
	bool hidden; // Whether the entity is hidden from view or not (all entities start as hidden)

public:
	Entity(float xCoord, float yCoord, Sprite s, const string& t=""s); // Single Sprite Constructors
	Entity(Corner cor, Sprite s, const string& t=""s); // Aligns to the specified corner
	Entity(Sprite s, const string& t=""s); // like the above but goes offscreen

	Entity(float xCoord, float yCoord, const vector<Sprite>& c, const string& t=""s); // Costume List Constructors
	Entity(Corner cor, const vector<Sprite>& c, const string& t=""s);
	Entity(const vector<Sprite>& c, const string& t=""s);
	Entity(const string& t=""s); // uses the empty sprite, rendered offscreen (default constructor)
	~Entity(); // deallocates the components

	Entity(const Entity&); 
	Entity& operator=(const Entity&); // Copying and copy assignment discards an entity's components
	
	Entity(Entity&&) = default; 
	Entity& operator=(Entity&&) = default; // Moving is fine

	float getX() const; // Self-explanatory getters and setters
	float getY() const;
	Entity& setX(float amount);
	Entity& setY(float amount); 
	Entity& setCoords(float amount1, float amount2);
	Entity& setCoords(Point p);
	Entity& changeX(float amount); 
	Entity& changeY(float amount);
	Point centerOf() const;

	Entity& setSize(float amount);
	Entity& changeSize(float amount);
	float getSize() const;

	int getWidth() const; 
	int getHeight() const; 
	Texture* getTexture() const; 
	Sprite getCostume() const;
	Rect getRect() const;

	Entity& initTransform(); // Initializes the transform to null
	optional<RenderTransform>& getTransform(); // The caller must unpack the value
	const optional<RenderTransform>& getTransform() const; // Subscript-adjacent operators often come in pairs
	Entity& disableTransform(); // Removes the transform
 
	Entity& setCostume(int k); // Assigns the costume index and updates the currentSprite
	Entity& setCostume(Sprite s); // Assigns currentSprite, temporarily invalidating costume_index (which is set to -1)
	Entity& nextCostume(); // Switches to the next costume, wrapping around if at costumes.size() - 1 (UB if index = -1)
	Entity& prevCostume(); // Switches to the previous costume, wrapping around if at 0 (UB if index = -1)
	Entity& cycleToCostume(Sprite s); // Cycles through the costumes until a matching sprite is found
	Entity& addAltCostume(Sprite s); // Adds a sprite to the costume vector at the end
	Entity& setCostumes(const vector<Sprite>& c); // Assigns the costume vector

	Entity& tagAs(const string& t); // Assigns a tag
	Entity& tagAs(const char* t);
	const string& getTag() const; // Returns the tag
	Entity& removeTag(); // Removes the tag (i.e. sets it to "")

	Entity& addComponent(Component::ComponentType* cmptr); // Note that components cannot be removed
	void update(); // Runs the functionality of each component
	Entity& send(Broadcast b); // Sends a broadcast
	Entity& setGameData(void* data);
	void* getGameData(); // Sets and gets gameData

	Entity& instantiate(int l=1); // Adds to the renderqueue via a broadcast
	Entity& instantiateHidden(int l=1); // Adds to the renderqueue, hidden
	Entity& hide(); // Temporarily removes from view
	Entity& show(); // Teverses hide()
	Entity& toggle(); // Switches from shown to hidden or vice versa.
	Entity& remove(); // Removes from the renderqueue
	Entity& removeDestroy(); // Removes from the renderqueue and deallocates (use only for dynamically allocated entities)

	friend class RenderQueue;
	friend class RenderWindow;
}; 

using TrivialEntityMemFn = Entity& (Entity::*)();
using FloatEntityMemFn = Entity& (Entity::*)(float);
}
