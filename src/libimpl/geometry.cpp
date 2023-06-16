#include "libraries/geometry.hpp"

#include <cmath>
#include <iostream>

using namespace SPP;

float geometry::dist(float x1, float x2, float y1, float y2) 
{
	return sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)); // should be std::sqrtf; this is a known bug in gcc
}

float geometry::dist(Point a, Point b)
{
	return dist(a.x, b.x, a.y, b.y);
}

float geometry::containsPoint(Rect r, Point p)
{
	return r.x <= p.x && p.x <= r.x + r.w && r.y <= p.y && p.y <= r.y + r.h;
}

float geometry::containsPoint(Circle c, Point p)
{
	return dist(c.centre, p) <= c.radius;
}

bool geometry::colliding(Rect a, Rect b)
{
	// return SDL_HasIntersectionF(&a, &b);
	// Turns out I'm on an outdated distro of SDL2 because URG was started before April 2022
	// The above function doesn't exist, so we'll have to roll our own until I get around to updating it

	float al = a.x;
	float ar = a.x + a.w;
	float at = a.y;
	float ab = a.y + a.h;

	float bl = b.x;
	float br = b.x + b.w;
	float bt = b.y;
	float bb = b.y + b.h;

	return (bl > ar || br < al || bt > ab || bb < at ? false : true);
}

bool geometry::colliding(Circle a, Circle b)
{
	return dist(a.centre, b.centre) <= a.radius + b.radius;
}

bool geometry::colliding(Circle c, Rect r)
{
	// some complicated algorithm that I haven't implemented yet...

	return false;
}
bool geometry::colliding(Rect r, Circle c) { return colliding(c, r); }

float geometry::degrees_to_radians(float d)
{
	return d * (PI / 180.0f);
}

float geometry::radians_to_degrees(float d)
{
	return d * (180.0f / PI);
}

Point geometry::getCorner(Rect r, Corner crnr)
{
	switch (crnr) {
		case Corner::TOP_LEFT:
			return {r.x,r.y};
		case Corner::TOP_RIGHT:
			return {r.x + r.w, r.y};
		case Corner::BOTTOM_LEFT:
			return {r.x, r.y + r.h};
		case Corner::BOTTOM_RIGHT:
			return {r.w + r.w, r.y + r.h};
	}
	return {0,0}; // shuts up the compiler but will never happen
}

Point geometry::alignWith(Rect inner, Rect outer)
{
	if (inner.w > outer.w) {
		std::cout << "cannot align: too wide!" << '\n';
	} else if (inner.h > outer.h) {
		std::cout << "cannot align: too tall!" << '\n';
	} else {
		return {outer.x + 0.5f*(outer.w - inner.w), outer.y + 0.5f*(outer.h - inner.h)};
	}
	return getCorner(outer, Corner::TOP_LEFT);
}

Point geometry::randomPoint(Rect region)
{
	float x = rng::randomValue(region.x, region.x + region.w);
	float y = rng::randomValue(region.y, region.y + region.h);
	return {x, y};
}

Point geometry::randomPoint(Circle region)
{
	float theta = rng::randomValue(-WEST, WEST);
	float r = rng::randomValue(0, region.radius);
	return {r*cosf(theta), r*sinf(theta)};
}

Point geometry::randomPoint(Point origin, float radius)
{
	return randomPoint(Circle(origin, radius));
}