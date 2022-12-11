#include "./PerlinGenerator/PerlinGenerator.hpp"
#include "./mesher/mesher.hpp"

#include <fstream>
#include <chrono>
#include <string>

#include <fmt/format.h>


int main(int argc, char const *argv[])
{
	int width = 100;
	int height = 100;

	tgen::PerlinGenerator pg;
	auto points = pg.GenerateTerrain(width, height, 0u, 4, 10, 0.1);

	const auto ts = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	tgen::Mesher mr;
	tgen::Mesh m = mr.triangulate(points);

	std::string filename = fmt::format("../data/{}.off", ts);
	
	std::ofstream out(filename);
	fmt::print("{} created\n", filename);

	out << m;

	return 0;
}