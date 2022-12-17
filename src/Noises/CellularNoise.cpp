#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"
#include <cmath>

class tgen::CellularNoise: public tgen::Noise{

	FastNoiseLite cellular;
public:

	CellularNoise(unsigned int seed) : Noise(seed){
		cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
		cellular.SetSeed(seed);
	}

	double generateNoise(double x, double y, int octaves){
		//cellular.SetFractalOctaves(octaves);
		cellular.SetCellularJitter(3);
		cellular.SetFrequency(0.1);
		return cellular.GetNoise(x, y);
	}

}; 