#include "NoiseGenerator.hpp"
#include "../terrain_generation.hpp"
#include <fmt/format.h>

tgen::NoiseGenerator::NoiseGenerator(int noise, unsigned int seed){
	switch(noise){
	case 0:
		this->noise = new tgen::PerlinNoise(seed);
		break;
	case 1:
		this->noise = new tgen::CellularNoise(seed);
		break;
	case 2:
		this->noise = new tgen::OpenSimplexNoise(seed);
		break;
	case 3:
		this->noise = new tgen::FBMNoise(seed);
		break;
	}
}

std::vector<tgen::Point> tgen::NoiseGenerator::generatePoints(int width, int heigth, 
	int octaves, int amplitude, double frequency) {

	std::vector<Point> points;
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = noise->generateNoise((x * frequency), (y * frequency), octaves) * amplitude;
			// fmt::print("{} {}\n", elevation, y);
			points.push_back(Point(x, y, elevation));
		}
	}

	return points;
}

tgen::Point** tgen::NoiseGenerator::generatePointsMatrix(int width, int heigth, 
 	int octaves, int amplitude, double frequency) {

	Point** points = new Point*[width];
	for(int x=0 ; x < width ; x++){
		points[x]=new Point[heigth];
	}
	
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = (noise->generateNoise((x * frequency), (y * frequency), octaves)) * amplitude;
			points[x][y]=Point(x, y, elevation);
		}
	}
	return points;
}
