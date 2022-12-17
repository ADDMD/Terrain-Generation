#include "PerlinGenerator.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "./stb_perlin.h"
#include <cmath>

tgen::PerlinGenerator::PerlinGenerator(){}

std::vector<tgen::Point> tgen::PerlinGenerator::GenerateTerrain(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

	const siv::PerlinNoise perlin{ seed };

	std::vector<Point> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			//double noise = perlin.octave2D_01((x * frequency), (y * frequency), octaves) * amplitude;
			double oct=std::pow(2,octaves);
			double noise = stb_perlin_noise3((x * frequency), (y * frequency), 0,oct,oct,0) * amplitude;
			points.push_back(Point(x, y, noise));
		}
	}

	return points;
}