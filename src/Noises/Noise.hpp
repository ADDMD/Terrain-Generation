#ifndef NOISE_H
#define NOISE_H
#include "../terrain_generation.hpp"

class tgen::Noise {

public:
	
	unsigned int seed;
	int octaves;
	double frequency;

	Noise(unsigned int seed, int octaves, double frequency){
		this->seed = seed;
		this->octaves = octaves;
		this->frequency = frequency;
	}

	virtual double generateNoise(double x, double y) = 0;
};

#endif