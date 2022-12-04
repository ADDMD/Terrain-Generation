#include <iostream>
#include "../include/PerlinNoise/PerlinNoise.hpp"
#include <fmt/format.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Point_3.h>

typedef CGAL::Cartesian<double> kernel;
typedef CGAL::Point_3<kernel> point;

int main()
{
	const siv::PerlinNoise::seed_type seed = 123456u;

	const siv::PerlinNoise perlin{ seed };

	int width = 100;
	int height = 100;

	point points[width * height];
	
	for (int y = 0; y < width; ++y)
	{
		for (int x = 0; x < height; ++x)
		{
			const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
			points[(y + x) * y] = point(y, x, noise);
			fmt::print("{}\t", points[(y + x) * y].z());
		}

		fmt::print("\n");
	}
}