#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <vector>
#include "../terrain_generation.hpp"
#include "../Noises/PerlinNoise.cpp"
#include "../Noises/CellularNoise.cpp"
#include "../Noises/OpenSimplexNoise.cpp"
#include "../Noises/FBMNoise.cpp"
#include "../Noises/SimplexNoise.cpp"

class tgen::NoiseGenerator{

	Noise* noise;

public:
	NoiseGenerator(int noise, unsigned int seed);

	std::vector<Point> generatePoints(int width, int height, 
		int octaves, int amplitude, double frequency);

	Point** generatePointsMatrix(int width, int heigth, 
		int octaves, int amplitude, double frequency); 

};


#endif