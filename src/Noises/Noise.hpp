#ifndef NOISE_H
#define NOISE_H
#include "../terrain_generation.hpp"

class tgen::Noise {

public:
	
	unsigned int seed;
	int octaves;
	int amplitude;
	double frequency;

	Noise(unsigned int seed, int octaves, int amplitude, double frequency){
		this->seed = seed;
		this->octaves = octaves;
		this->amplitude = amplitude;
		this->frequency = frequency;
	}

	virtual double generateNoise(double x, double y) = 0;
};

#endif