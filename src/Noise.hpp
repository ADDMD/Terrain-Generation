#ifndef NOISE_H
#define NOISE_H
#include "./terrain_generation.hpp"

class tgen::Noise {
public:
	virtual double generateNoise(double x, double y) = 0;
};

#endif