#include "../../include/PerlinNoise/PerlinNoise.hpp"
#include <CGAL/Cartesian.h>
#include <CGAL/Point_3.h>
#include "PerlinGenerator.hpp"

std::vector<point> PerlinGenerator::GenerateTerrain(int width, int heigth, unsigned int seed){

	const siv::PerlinNoise perlin{ seed };

	std::vector<point> points;
	for( int x = 0 ; x < width ; x++){
		for ( int y = 0 ; y < heigth ; y++){
			points.push_back(point(x, y, perlin.octave2D_01((x * 0.01), (y * 0.01), 4)));
		}
	}
	return points;
}