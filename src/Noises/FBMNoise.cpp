#include "Noise.hpp"
//include dell fbm

class tgen::FBMNoise: public tgen::Noise{
public:
	FBMNoise(unsigned int seed, int octaves, int amplitude, double frequency) : Noise(seed, octaves, amplitude, frequency){}

	double generateNoise(double x, double y){
		return 0;
	}
}; 