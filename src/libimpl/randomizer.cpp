#include "randomizer.hpp"

using namespace SPP;

int randomization::randomInt(int a, int b)
{
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(generator);
}

float randomization::randomValue(float a, float b)
{
	std::uniform_real_distribution<float> distribution(a, b);
	return distribution(generator);
}

bool randomization::probabilityChance(float p)
{
	return randomValue(0, 1) <= p;
}

bool randomization::percentChance(float p)
{
	return probabilityChance(p / 100.0f);
}