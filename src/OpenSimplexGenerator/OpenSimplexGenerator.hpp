/*#ifndef OPENSIMPLEXGENERATOR_H
#define OPENSIMPLEXGENERATOR_H

#include <vector>
#include <CGAL/Point_3.h>
#include <CGAL/Cartesian.h>
#include "../../include/OpenSimplex/OpenSimplex.h"
#include "../terrain_generation.hpp"


class tgen::OpenSimplexGenerator{
public:
	OpenSimplexGenerator();
	std::vector<Point> GenerateTerrain(int width, int height, 
		unsigned int seed, int octaves, int amplitude, double frequency);
};


#endif*/