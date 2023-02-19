#include "Noise.hpp"
#include "../../include/PerlinNoise/PerlinNoise.hpp"

class tgen::PerlinNoise: public tgen::Noise{
	
	siv::PerlinNoise perlin;

public:
	PerlinNoise(unsigned int seed, int octaves, double frequency) : Noise(seed, octaves, frequency){
		perlin = siv::PerlinNoise(seed);
	}

	double generateNoise(double x, double y){
		return perlin.octave2D_11(x * this->frequency, y * this->frequency, this->octaves);
	}
}; 