#include "SimplexGenerator.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "../include/SimplexNoise/stb_perlin.h"
#include <cmath>

tgen::SimplexGenerator::SimplexGenerator(){}

std::vector<tgen::Point> tgen::SimplexGenerator::generateTerrain(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

	std::vector<Point> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double oct=std::pow(2,octaves);
			double noise = stb_perlin_noise3_seed((x * frequency), (y * frequency), 0,oct,oct,0,seed) * amplitude;
			points.push_back(Point(x, y, noise));
		}
	}

	return points;
}

double** tgen::SimplexGenerator::generateTerrainMatrix(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

	double** points = new double*[width];
	for(int x=0 ; x < width ; x++){
		points[x]=new double[heigth];
	}
	
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double oct=std::pow(2,octaves);
			double noise = stb_perlin_noise3_seed((x * frequency), (y * frequency), 0,oct,oct,0,seed) * amplitude;
			points[x][y]=noise;
		}
	}
	return points;
}