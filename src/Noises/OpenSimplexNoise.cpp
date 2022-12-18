#include "Noise.hpp"
#include "../../include/OpenSimplex/OpenSimplex.h"

class tgen::OpenSimplexNoise: public tgen::Noise{

	OpenSimplex::Context context;

public:
	OpenSimplexNoise(unsigned int seed, int octaves, int amplitude, double frequency) : Noise(seed, octaves, amplitude, frequency){
		OpenSimplex::Seed::computeContextForSeed(context, seed);
	}

	double generateNoise(double x, double y){
		return static_cast<double>(OpenSimplex::Noise::noise2(context, x * this->frequency, y * this->frequency)) * this->amplitude;
	}

}; 