#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

#include "entity.hpp"

using namespace SPP;

Entity::Entity(float xCoord, float yCoord, const vector<Sprite>& c, const string& t)
: x{xCoord}, y{yCoord}, size{1}
, currentSprite{c.front()}, costumes{c}, costume_index{0}
, gameData{nullptr}, tag{t}, hidden{true}
{ ; }

Entity::Entity(Corner cor, const vector<Sprite>& c, const string& t) : Entity(c, t)
{ 
	switch(cor) {
		case Corner::TOP_LEFT:
			setCoords(0, 0);
			break;
		case Corner::TOP_RIGHT:
			setCoords(FULLSCREEN_WIDTH - currentSprite.width, 0);
			break;
		case Corner::BOTTOM_LEFT:
			setCoords(0, FULLSCREEN_HEIGHT - currentSprite.height);
			break;
		case Corner::BOTTOM_RIGHT:
			setCoords(FULLSCREEN_WIDTH - currentSprite.width, FULLSCREEN_HEIGHT - currentSprite.height);
			break;
	} 
}

Entity::Entity(const vector<Sprite>& c, const string& t) : Entity(OFFSCREEN.x, OFFSCREEN.y, c, t)
{ ; }

Entity::Entity(float xCoord, float yCoord, Sprite s, const string& t) : Entity(xCoord, yCoord, vector{s}, t)
{ ; } // costumes is a vector of length 1

Entity::Entity(Corner cor, Sprite s, const string& t) : Entity(cor, vector{s} , t)
{ ; }

Entity::Entity(Sprite s, const string& t) : Entity(vector{s}, t)
{ ; }

Entity::Entity(const string& t) : Entity(emptySprite, t)
{ ; }

Entity::~Entity()
{ ; } // Component unique_ptrs are deallocated automatically

float Entity::getX() const { return x; }
float Entity::getY() const { return y; }

Entity& Entity::setX(float amount) { x = amount; return *this; }
Entity& Entity::setY(float amount) { y = amount; return *this; }

Entity& Entity::setCoords(float amount1, float amount2) { x = amount1; y = amount2; return *this; }
Entity& Entity::setCoords(Point p) { x = p.x; y = p.y; return *this; }

Point Entity::centerOf() const { return {x,y}; }

Entity& Entity::changeX(float amount) { x += amount; return *this; }
Entity& Entity::changeY(float amount) { y += amount; return *this; }

Entity& Entity::setSize(float amount) { size = amount; return *this; }
Entity& Entity::changeSize(float amount) { size += amount; return *this; }

float Entity::getSize() const { return size; }

int Entity::getWidth() const { return currentSprite.width; }
int Entity::getHeight() const { return currentSprite.height; }
Texture* Entity::getTexture() const { return currentSprite.texture; }
Rect Entity::getRect() const 
{ 
	return {x, y, static_cast<float>(currentSprite.width), static_cast<float>(currentSprite.height)}; 
}

Entity& Entity::initTransform() { transform = nullTransform; return *this; }
optional<RenderTransform>& Entity::getTransform() { return transform; }
const optional<RenderTransform>& Entity::getTransform() const { return transform; }
Entity& Entity::disableTransform() { transform.reset(); return *this; }

Entity& Entity::setCostume(int k) 
{
	costume_index = k; 
	currentSprite = costumes.at(costume_index); 
	return *this; 
}
Entity& Entity::setCostume(Sprite s)
{
	costume_index = -1;
	currentSprite = s;
	return *this;
}
Entity& Entity::nextCostume() 
{ 
	++costume_index %= costumes.size(); 
	currentSprite = costumes.at(costume_index); 
	return *this; 
}
Entity& Entity::prevCostume() 
{ 
	costume_index = (costume_index == 0 ? costumes.size() - 1 : costume_index - 1);
	currentSprite = costumes.at(costume_index); 
	return *this; 
}
Entity& Entity::cycleToCostume(Sprite s)
{
	auto newCostume = std::find(costumes.begin(), costumes.end(), s);
	if (newCostume != costumes.end()) setCostume(newCostume - costumes.begin());
	return *this;
}

Sprite Entity::getCostume() const { return currentSprite; }
Entity& Entity::addAltCostume(Sprite s) { costumes.push_back(s); return *this; } 
Entity& Entity::setCostumes(const vector<Sprite>& c) { costumes = c; return *this; }

Entity& Entity::tagAs(const string& t) { tag = t; return *this; }
Entity& Entity::tagAs(const char* t) { tag = string(t); return *this; }
const string& Entity::getTag() const { return tag; }
Entity& Entity::removeTag() { tag = ""s; return *this; }

Entity& Entity::addComponent(Component::ComponentType* cmptr) 
{
	components.emplace_back(unique_ptr<Component::ComponentType>(cmptr)); 
	return *this; 
}

void Entity::update() 
{
	for (auto& c : components) {
		c->update(*this);
	}
}

Entity& Entity::send(Broadcast b) { BroadcastQueue::receive(b); return *this; }
Entity& Entity::setGameData(void* data) { gameData = data; return *this; }
void* Entity::getGameData() { return gameData; }

Entity& Entity::instantiate(int l)
{ 
	send({SHOW_ENTITY, this, l}); 
	hidden = false;
	return *this; 
}
Entity& Entity::instantiateHidden(int l)
{ 
	send({SHOW_ENTITY, this, -l}); 
	hidden = true;
	return *this; 
}

Entity& Entity::hide()
{ 
	send({CHANGE_VISIBILITY_ENTITY, this, (int)'h'});
	hidden = true;
	return *this;  
}
Entity& Entity::show() 
{ 
	send({CHANGE_VISIBILITY_ENTITY, this, (int)'s'});
	hidden = false;
	return *this;  
} 
Entity& Entity::toggle() 
{ 
	send({CHANGE_VISIBILITY_ENTITY, this, (int)'t'});
	hidden = false;
	return *this;  
} 

Entity& Entity::remove()
{
	send({HIDE_ENTITY, this});
	hidden = true;
	return *this; 
}
Entity& Entity::removeDestroy()
{
	send({KILL_ENTITY, this});
	hidden = true;
	return *this; 
}
