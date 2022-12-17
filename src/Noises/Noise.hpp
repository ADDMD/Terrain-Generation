#ifndef NOISE_H
#define NOISE_H
#include "../terrain_generation.hpp"

class tgen::Noise {

public:
	
	unsigned int seed;

	Noise(unsigned int seed){
		this->seed = seed;
	}

	virtual double generateNoise(double x, double y, int octaves) = 0;
};

#endif