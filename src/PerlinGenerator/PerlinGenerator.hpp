#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <vector>
#include <CGAL/Point_3.h>
#include <CGAL/Cartesian.h>
#include "../../include/PerlinNoise/PerlinNoise.hpp"

typedef CGAL::Cartesian<double> kernel;
typedef CGAL::Point_3<kernel> point;

class PerlinGenerator{
public:
	PerlinGenerator();
	std::vector<point> GenerateTerrain(int width, int height, unsigned int seed, int octaves, int amplitude, double frequency);
};

#endif