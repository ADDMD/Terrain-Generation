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
	NoiseGenerator(int noise, unsigned int seed, int octaves, int amplitude, double frequency);

	std::vector<Point> generatePoints(int width, int height);

	Point** generatePointsMatrix(int width, int heigth); 

};


#endif