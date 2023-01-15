#include "NoiseGenerator.hpp"
#include "../terrain_generation.hpp"

#include <fmt/format.h>

tgen::NoiseGenerator::NoiseGenerator(int noise, unsigned int seed, int octaves, int amplitude, double frequency){
	switch(noise){
	case 0:
		this->noise = new tgen::PerlinNoise(seed, octaves, amplitude, frequency);
		break;
	case 1:
		this->noise = new tgen::SimplexNoise(seed, octaves, amplitude, frequency);
		break;
	case 2:
		this->noise = new tgen::OpenSimplexNoise(seed, octaves, amplitude, frequency);
		break;
	case 3:
		this->noise = new tgen::CellularNoise(seed, octaves, amplitude, frequency);
		break;
	case 4:
		this->noise = new tgen::FBMNoise(seed, octaves, amplitude, frequency);
		break;
	}
}

std::vector<tgen::Point> tgen::NoiseGenerator::generatePoints(int width, int heigth) {

	std::vector<Point> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = noise->generateNoise(x, y);
			points.push_back(Point(x, y, elevation));
		}
	}

	return points;
}

double** tgen::NoiseGenerator::generateMap(int width, int heigth) {

	double** points = new double*[width];
	for(int x=0 ; x < width ; x++){
		points[x]=new double[heigth];
	}
	
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = noise->generateNoise(x, y);
			points[x][y] = elevation;
		}
	}
	return points;
}
