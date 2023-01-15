#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include "../terrain_generation.hpp"
#include "../Noises/PerlinNoise.cpp"
#include "../Noises/CellularNoise.cpp"
#include "../Noises/OpenSimplexNoise.cpp"
#include "../Noises/FBMNoise.cpp"
#include "../Noises/SimplexNoise.cpp"
#include "../Utility/Logger.hpp"

#include <vector>


class tgen::NoiseGenerator{

	std::string name = "NoiseGenerator";
	TGENLogger logger = TGENLogger(name);

protected:
	Noise* noise;

public:
	NoiseGenerator(int noise, unsigned int seed, int octaves, int amplitude, double frequency);

	std::vector<Point> generatePoints(int width, int height);

	double** generateMap(int width, int heigth);

};


#endif