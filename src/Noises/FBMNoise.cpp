#include "Noise.hpp"
#include "../../include/FastNoise/FastNoiseLite.h"

class tgen::FBMNoise: public tgen::Noise{
	FastNoiseLite fbm;
public:

	FBMNoise(unsigned int seed, int octaves, int amplitude, double frequency) : Noise(seed, octaves, amplitude, frequency){
		fbm.SetSeed(seed);
		fbm.SetNoiseType(FastNoiseLite::NoiseType_Value);
		fbm.SetFractalOctaves(octaves);
		fbm.SetFrequency(frequency);
		fbm.SetFractalType(FastNoiseLite::FractalType_FBm);
	}

	double generateNoise(double x, double y){
		return fbm.GetNoise(x, y) * this->amplitude;
	}
}; 