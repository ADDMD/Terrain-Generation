#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"

class tgen::SimplexNoise: public tgen::Noise{
	
	FastNoiseLite simplex;

public:
	SimplexNoise(unsigned int seed) : Noise(seed){
		simplex.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		simplex.SetSeed(seed);
	}

	double generateNoise(double x, double y, int octaves){
		simplex.SetFractalOctaves(octaves);
		return simplex.GetNoise(x, y);
	}
}; 