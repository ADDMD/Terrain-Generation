#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"
//include dell fbm

class tgen::FBMNoise: public tgen::Noise{
	FastNoiseLite fbm;
public:
	FBMNoise(unsigned int seed) : Noise(seed){
		fbm.SetSeed(seed);
		fbm.SetNoiseType(FastNoiseLite::NoiseType_Value);
		fbm.SetFractalType(FastNoiseLite::FractalType_FBm);
	}

	double generateNoise(double x, double y, int octaves){
		return fbm.GetNoise(x, y);
	}
}; 