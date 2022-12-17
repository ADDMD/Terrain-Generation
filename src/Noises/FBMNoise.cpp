#include "Noise.hpp"
//include dell fbm

class tgen::FBMNoise: public tgen::Noise{
public:
	FBMNoise(unsigned int seed) : Noise(seed){}

	double generateNoise(double x, double y, int octaves){
		return 0;
	}
}; 