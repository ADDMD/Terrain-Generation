#include "Noise.hpp"
#include "../include/PerlinNoise/PerlinNoise.hpp"

class tgen::PerlinNoise: public Noise{
public:
	PerlinNoise(unsigned int seed, int oct){
		perlin = siv::PerlinNoise(seed);
		octaves = oct;
		}

	double generateNoise(double x, double y){
		return perlin.octave2D_01(x, y, octaves);
	}

private:
	siv::PerlinNoise perlin;
	int octaves;
}; 