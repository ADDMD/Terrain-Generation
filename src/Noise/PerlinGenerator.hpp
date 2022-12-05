#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <vector>
#include <CGAL/Point_3.h>
#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian<double> kernel;
typedef CGAL::Point_3<kernel> point;

class PerlinGenerator{
public:
	PerlinGenerator();
	std::vector<point> GenerateTerrain(int width, int height, unsigned int seed);
};

#endif