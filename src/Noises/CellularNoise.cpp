#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"
#include <cmath>

class tgen::CellularNoise: public tgen::Noise{

	FastNoiseLite cellular;
public:

	CellularNoise(unsigned int seed, int octaves, int amplitude, double frequency) : Noise(seed, octaves, amplitude, frequency){
		cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
		cellular.SetSeed(seed);
		cellular.SetFractalOctaves(octaves);
		cellular.SetFrequency(frequency);
	}

	double generateNoise(double x, double y){
		return cellular.GetNoise(x, y) * this->amplitude;
	}

}; 