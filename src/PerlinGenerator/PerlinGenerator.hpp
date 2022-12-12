#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <vector>
#include <CGAL/Point_3.h>
#include <CGAL/Cartesian.h>
#include "../../include/PerlinNoise/PerlinNoise.hpp"
#include "../terrain_generation.hpp"


class tgen::PerlinGenerator{
public:
	PerlinGenerator();
	std::vector<Point> GenerateTerrain(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);
};


#endif