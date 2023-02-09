#include "NoiseGenerator.hpp"
#include "../terrain_generation.hpp"

#include <fmt/format.h>

tgen::NoiseGenerator::NoiseGenerator(int noise, unsigned int seed, int octaves, int amplitude, double frequency){
	switch(noise){
	case 0:
		this->noise = new tgen::PerlinNoise(seed, octaves, frequency);
		break;
	case 1:
		this->noise = new tgen::SimplexNoise(seed, octaves, frequency);
		break;
	case 2:
		this->noise = new tgen::OpenSimplexNoise(seed, octaves, frequency);
		break;
	case 3:
		this->noise = new tgen::CellularNoise(seed, octaves, frequency);
		break;
	case 4:
		this->noise = new tgen::FBMNoise(seed, octaves, frequency);
		break;
	}

	this->amplitude = amplitude;
}

std::vector<tgen::Point_3> tgen::NoiseGenerator::generatePoints(int width, int height) {

	std::vector<Point_3> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < height ; y++){
			double elevation = noise->generateNoise(x, y) * this->amplitude;
			points.push_back(Point_3(x, y, elevation));
		}
	}

	return points;
}

tgen::Matrix<tgen::FT> tgen::NoiseGenerator::generateMap(int width, int height, double exp_elevation) {

	Matrix<FT> points = generateMatrix<FT>(width, height);
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < height; y++){
			double elevation = (noise->generateNoise(x, y) + 1) / 2;
			points[x][y] = std::pow(elevation, exp_elevation) * this->amplitude;
		}
	}
	return points;
}
