#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <vector>
#include "SimplexGenerator.hpp"
#include "../terrain_generation.hpp"


class tgen::SimplexGenerator{
public:
	SimplexGenerator();
	std::vector<Point> GenerateTerrain(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);
};


#endif