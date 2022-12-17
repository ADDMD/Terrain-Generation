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

tgen::Point** tgen::PerlinGenerator::generateTerrainMatrix(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

	Point** points = new Point*[width];
	for(int x=0 ; x < width ; x++){
		points[x]=new Point[heigth];
	}

	tgen::Noise* noise = new tgen::PerlinNoise(seed, octaves);
	
	for( int x = 0 ; x < width; x++){
		for ( int y = 0 ; y < heigth ; y++){
			double elevation = (noise->generateNoise((x * frequency), (y * frequency))) * amplitude;
			points[x][y]=Point(x, y, elevation);
		}
	}
	return points;
}
