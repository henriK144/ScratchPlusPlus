#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <functional>
#include <iostream>

#include "renderqueue.hpp"

using SPP::RenderQueue;

RenderQueue::RenderQueue()
{ ; }

RenderQueue::RenderQueue(Entity* bg)
{
	display(bg, 0);
}

RenderQueue& RenderQueue::setBG(Entity* e) 
{ 
	layers[0].clear();
	display(e, 0); 
	return *this; 
}

RenderQueue& RenderQueue::addBG(Entity* e)
{
	display(e, 0);
	return *this;
}

RenderQueue& RenderQueue::display(Entity* e, int l)
{
	if (e) {
		layers[l].push_back(e);
	}
	return *this;
}

RenderQueue& RenderQueue::remove_when(EntityPredicate p, bool destroy)
{
	for (auto& [l,v] : layers) {
		auto toRemove = std::stable_partition(v.begin(), v.end(), std::not_fn(p)); // send those to be removed to back of queue

		if (destroy) {
			for (auto it = toRemove; it != v.end(); ++it) {
				dustbin.push_back(*it); // send to dustbin if destroying
			}
		} 

		v.erase(toRemove, v.end()); // then remove from queue
	}
	return *this;
}

RenderQueue& RenderQueue::remove(Entity* e, bool destroy) 
{ 
	return remove_when(([e](Entity* eptr) { return eptr == e; }), destroy); 
}

RenderQueue& RenderQueue::remove(const string& t, bool destroy)
{ 
	return remove_when(([t](Entity* eptr) { return eptr->getTag() == t; }), destroy); 
}

RenderQueue& RenderQueue::clear(bool destroy)
{ 
	return remove_when(([](Entity* eptr) { return true; }), destroy); 
}

RenderQueue& RenderQueue::load(Scene& s)
{
	int layer = 0;

	for (auto it = s.schema.begin(); it != s.schema.end(); ++it) {
		if (*it == nullptr) { // layer delimiter
			++layer;
		} else { // regular entity
			display(*it, layer);
		}
	}

	return *this;
}

RenderQueue& RenderQueue::changeVisibility(Entity* e, char c)
{
	for (auto& [l,v] : layers) {
		bool condition = (c == 'h' ? (l > 0) : c == 's' ? (l < 0) : true);
		auto toAlter = std::find(v.begin(), v.end(), e);
		if (toAlter != v.end() && condition) { // if found and the entity is shown/hidden/whatever
			display(*toAlter, -l); // then add to negative layer
			v.erase(toAlter); // and remove from current layer
		}
	}
	return *this;
}

void RenderQueue::dispose()
{
	for (auto eptr : dustbin) {
		delete eptr;
		eptr = nullptr; // just in case
	}
	dustbin.clear();
}

void RenderQueue::debug()
{
	std::cout << "Contents of RenderQueue" << ":\n";
	for (auto& [l,v] : layers) {
		std::cout << "Layer " << l << ":\n";
		for (auto e : v) {
			std::cout << "Entity at " << e << " is being rendered" << '\n';
		}
	}
	std::cout << "Dustbin contains " << dustbin.size() << " entities:\n";
	for (auto eptr : dustbin) {
		std::cout << "entity: " << eptr << '\n';
	}
}
