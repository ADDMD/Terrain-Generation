#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"

class tgen::SimplexNoise: public tgen::Noise{
	
	FastNoiseLite simplex;

public:
	SimplexNoise(unsigned int seed, int octaves, double frequency) : Noise(seed, octaves, frequency){
		simplex.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		simplex.SetSeed(seed);
		simplex.SetFractalOctaves(octaves);
		simplex.SetFrequency(frequency);
	}

	double generateNoise(double x, double y){
		return simplex.GetNoise(x, y);
	}
}; 