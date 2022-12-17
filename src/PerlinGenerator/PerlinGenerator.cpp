#include "PerlinGenerator.hpp"
#include <fmt/format.h>

tgen::PerlinGenerator::PerlinGenerator(){}

std::vector<tgen::Point> tgen::PerlinGenerator::generateTerrain(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

	tgen::Noise* noise = new tgen::PerlinNoise(seed, octaves);

	std::vector<Point> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = noise->generateNoise((x * frequency), (y * frequency)) * amplitude;
			// fmt::print("{} {}\n", elevation, y);
			points.push_back(Point(x, y, elevation));
		}
	}

	return points;
}