#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sdltypes.hpp"
#include "sprite.hpp"
#include "randomizer.hpp"

namespace SPP {
namespace rng = randomization;
namespace geometry
{ // A set of constants and functions for all of your geometric needs
	static constexpr float PI = 3.14159265;
	static constexpr float EAST = 0, NORTH = PI / 2, WEST = PI, SOUTH = 3*PI / 2;
	static constexpr float NORTHEAST = PI / 4, NORTHWEST = 3*PI / 4, SOUTHWEST = 5*PI / 4, SOUTHEAST = 7*PI / 4;

	enum class Side {LEFT, RIGHT, TOP, BOTTOM};
	enum class Direction {LEFT, RIGHT, UP, DOWN}; // enum classes prevent name clashes
	enum class Corner {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};
	
	struct Circle
	{
		Point centre;
		float radius;

		Circle(Point p, float r) : centre{p}, radius{r} {}
		Circle(float x, float y, float r) : Circle(Point{x,y}, r) {}
	}; // Can be used for circular regions or hitboxes (such as would befit a circular object)

	float dist(float x1, float x2, float y1, float y2); // For proximity sensing
	float dist(Point a, Point b);

	float containsPoint(Rect r, Point p); // Checks if a point is in a Rect
	float containsPoint(Circle c, Point p); // Checks if a point is in a Circle

	bool colliding(Rect a, Rect b); // Checks if two Rects intersect
	bool colliding(Circle a, Circle b); // Checks if two Circles intersect
	bool colliding(Circle c, Rect r); // Checks if a Circle intersects a Rect
	bool colliding(Rect r, Circle c); // Checks if a Rect intersects a Circle

	float degrees_to_radians(float d); // For rotation animations, I guess?
	float radians_to_degrees(float d);

	float angleBetween(Point a, Point b); // Returns the angle of elevation/depression from a to b, in (-pi, pi]

	Point getCorner(Rect r, Corner crnr); // Returns the specified corner of a rect

	Point alignWith(Rect inner, Rect outer); // Returns the point p such that if 
	// inner is moved to put its top left corner at p then it will be perfectly centred within outer.
	// (This is only possible if inner.w <= outer.w and inner.h <= outer.h; if this is not the case then
	// getCorner(outer, Corner::TOP_LEFT) is the return value.)
	
	Point randomPoint(Rect region); // Returns a random point (according to a uniform distribution) in region
	Point randomPoint(Circle region);
	Point randomPoint(Point origin, float radius);
}
}
