#include "Noise.hpp"
#include "../../include/PerlinNoise/PerlinNoise.hpp"

class tgen::PerlinNoise: public tgen::Noise{
	
	siv::PerlinNoise perlin;

public:
	PerlinNoise(unsigned int seed, int octaves, int amplitude, double frequency) : Noise(seed, octaves, amplitude, frequency){
		perlin = siv::PerlinNoise(seed);
	}

	double generateNoise(double x, double y, int octaves){
		return perlin.octave2D_11(x, y, octaves);
	}
}; 