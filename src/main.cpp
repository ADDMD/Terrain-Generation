#include <iostream>
#include "PerlinNoise.hpp"
#include <fmt/format.h>

int main()
{
	const siv::PerlinNoise::seed_type seed = 123456u;

	const siv::PerlinNoise perlin{ seed };
	
	for (int y = 0; y < 100; ++y)
	{
		for (int x = 0; x < 100; ++x)
		{
			const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
			
			fmt::print("{}\t", noise);
		}

		fmt::print("\n");
	}
}