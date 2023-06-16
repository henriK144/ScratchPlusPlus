#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "types/sdltypes.hpp"
namespace SPP {
class Entity;

namespace Component
{ // This namespace contains the various kinds of components.
	class ComponentType
	{ // An object that provides arbitrary functionality to an Entity.
	  // You can create more of them if you want to.
	public:
		virtual ~ComponentType() {}
		virtual void update(Entity& e) = 0;
	};

	class Collision: public ComponentType
	{ // Detects and handles collision between entities
	protected:
		//Rect box;
	public:
		virtual ~Collision() {} 
	};

	class Physics: public ComponentType
	{ // Enables continuous motion and/or simulated physics
	public:
		virtual ~Physics() {}
	};

	class Input: public ComponentType
	{ // Detects and handles player input
	protected:
		SDL_Event capture;
	public:
		virtual ~Input() {}
	};
	
	class Script: public ComponentType
	{ // Performs an action each frame, such as for animation or AI
	public:
		virtual ~Script() {}
	};

	// (These do not implement update(Entity&), so they are still abstract. 
	// They must be inherited from to implement their update method, such as in this example:

	/*

	class RotationScript: public Script
	{
	private:
		static constexpr float rotation_speed = 3.5;
	public:
		RotationScript() {}
		virtual void update(Entity& e);
	};

	void RotationScript::update(Entity& e)
	{
		e.getTransform()->tilt += rotation_speed;
	}

	*/

	// The concrete derived classes are specific to each game and thus should probably be
	// placed in a nested namespace like "Component::MyGame" for better encapsulation.)

	// Also, because of how this is ge set up, a component 
	// will not automatically have access to the internal
	// data or state of another component; their primary purpose
	// is to modify the internal data or state of e.
	// If component A wants to modify component B (of the same entity)
	// then B should declare A as a friend and A should contain a pointer to B
	// and initialize it by way of a constructor argument.
	// A can then access B's internals in its update method as needed.
};
}
