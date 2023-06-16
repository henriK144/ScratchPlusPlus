#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <random>

using std::vector;
namespace SPP {
namespace randomization
{ // A set of constants and functions for various kinds of randomness
	static std::random_device rand_device;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 generator(rand_device()); // Standard mersenne_twister_engine seeded with rd()

    int randomInt(int a, int b); // Returns a random integer in [a,b]
    float randomValue(float a, float b); // Returns a random real number in [a,b)
    bool probabilityChance(float p); // Returns true with probability p and false with probability 1-p
    bool percentChance(float p); // Returns true with probability p% and false with probability (100-p)%

	template<typename T>
	class Randomizer 
	{ // A generic class that randomly selects a T according to a weighted distribution
	private:
		std::discrete_distribution<> weights;
		std::vector<T> items;

	public:
		Randomizer(const vector<T>& is, std::initializer_list<double> ws) 
		: weights{std::discrete_distribution(ws)}, items{is}
		{ ; }

		T get() // returns a T from the distribution
		{
			return items.at(weights(generator));
		}
	};
}
}
