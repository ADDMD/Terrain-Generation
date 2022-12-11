#include "./PerlinGenerator/PerlinGenerator.hpp"
#include "./mesher/mesher.hpp"

#include <fstream>
#include <chrono>
#include <string>

#include <fmt/format.h>

#include <CGAL/IO/PLY/PLY_writer.h>
#include <CGAL/IO/OBJ.h>

int main(int argc, char const *argv[])
{
	int width = 100;
	int height = 100;


	const auto seed = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	tgen::PerlinGenerator pg;
	auto points = pg.GenerateTerrain(width, height, seed, 4, 10, 0.1);

	tgen::Mesher mr;
	tgen::Mesh m = mr.triangulate(points);

	std::string filename = fmt::format("../data/{}.obj", seed);
	
	std::ofstream out(filename);
	fmt::print("{} created\n", filename);

	CGAL::IO::write_OBJ(out, m);

	// out << m;

	return 0;
}