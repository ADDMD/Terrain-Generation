#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <vector>
#include <CGAL/Point_3.h>
#include <CGAL/Cartesian.h>
#include "../terrain_generation.hpp"
#include "../PerlinNoise.cpp"


class tgen::PerlinGenerator{
public:
	PerlinGenerator();
	std::vector<Point> generateTerrain(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);
	Point** generateTerrainMatrix(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency); 
};


#endif