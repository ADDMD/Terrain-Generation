#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include "SimplexGenerator.hpp"
#include "../terrain_generation.hpp"

#include <vector>


class tgen::SimplexGenerator{
public:
	SimplexGenerator();
	std::vector<Point> generateTerrain(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);

	tgen::Point** generateTerrainMatrix(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);
};



#endif