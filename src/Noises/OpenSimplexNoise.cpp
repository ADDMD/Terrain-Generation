#include "Noise.hpp"
#include "../../include/OpenSimplex/OpenSimplex.h"

class tgen::OpenSimplexNoise: public tgen::Noise{

	OpenSimplex::Context context;

public:
	OpenSimplexNoise(unsigned int seed) : Noise(seed){
		OpenSimplex::Seed::computeContextForSeed(context, seed);
	}

	double generateNoise(double x, double y, int octaves){
		return static_cast<double>(OpenSimplex::Noise::noise2(context, x, y));
	}

}; 