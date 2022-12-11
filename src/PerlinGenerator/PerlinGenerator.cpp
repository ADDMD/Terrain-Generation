#include "PerlinGenerator.hpp"

PerlinGenerator::PerlinGenerator(){}

std::vector<point> PerlinGenerator::GenerateTerrain(int width, int heigth, unsigned int seed, int octaves, int amplitude, double frequency){

	const siv::PerlinNoise perlin{ seed };

	std::vector<point> points;
	for( int x = 0 ; x < width ; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double noise = perlin.octave2D_01((x * frequency), (y * frequency), octaves) * amplitude;
			points.push_back(point(x, y, noise));
		}
	}
	return points;
}