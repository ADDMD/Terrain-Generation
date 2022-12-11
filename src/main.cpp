#include <fmt/format.h>
#include "../src/PerlinGenerator/PerlinGenerator.hpp"
#include <vector>
#include <fstream>

int main(int argc, char const *argv[])
{
	int width = 1000;
	int height = 1000;

	PerlinGenerator pg;
	std::vector<point> points = pg.GenerateTerrain(width, height, 0u, 4, 10, 0.1);

	std::ofstream out("../test/data/points.txt");
	if(!out){
		exit(1);
	}
	int counter = 0;
	for(auto p: points){
		if(counter >= width){
			counter = 0;
			out << "\n";
		}
		out << fmt::format("{} ",p.z());
		counter++;
	}
	fmt::print("{}", points.size());
	out.close();
	fmt::print("Test\n");
	return 0;
}