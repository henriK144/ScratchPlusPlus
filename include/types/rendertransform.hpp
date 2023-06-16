#pragma once
namespace SPP {
struct Modulator 
{ // A struct for combining the parameters for an Entity's colour and alpha modulation
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 alpha;
};

inline Modulator emptyMods = {0xFF,0xFF,0xFF,0xFF};

inline bool operator==(Modulator m, Modulator n)
{ return (m.r == n.r && m.g == n.g && m.b == n.b && m.alpha == n.alpha); }

inline bool operator!=(Modulator m, Modulator n)
{ return !(m == n); }

struct RenderTransform
{  // A struct compartmentalizing Entity render transformation (more on this in entity.hpp)
	float tilt; // A rotational offset in radians, counterclockwise
	bool hInv; // Mirrors the entity across the y-axis
	bool vInv; // Mirrors the entity across the x-axis
	Modulator mods; // Applies the contained color and alpha modulations
};

inline RenderTransform nullTransform = {0, false, false, emptyMods};

inline bool operator==(RenderTransform r, RenderTransform s)
{ return (r.tilt == s.tilt && r.hInv == s.hInv && r.vInv == s.vInv && r.mods == s.mods); }

inline bool operator!=(RenderTransform r, RenderTransform s)
{ return !(r == s); }
}
