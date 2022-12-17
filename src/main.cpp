#include "./PerlinGenerator/PerlinGenerator.hpp"
#include "./OpenSimplexGenerator/OpenSimplexGenerator.hpp"
#include "./mesher/mesher.hpp"

#include <fstream>
#include <chrono>
#include <string>

#include <fmt/format.h>
#include <CGAL/Point_3.h>
#include <vector>
#include <iostream>

#include "./config.hpp"

int main(int argc, char const *argv[])
{
	config conf("../config.yaml");

	std::string w = conf["width"];
	std::string h = conf["height"];
	std::string a = conf["amplitude"];
	std::string o = conf["octaves"];
	std::string f = conf["frequency"];

	if(w == "" || h == "") return -1;

	int width = std::stoi(w);
	int height = std::stoi(h);
	int amplitude = std::stoi(a);
	int octaves = std::stoi(o);
	double frequency = std::stod(f);


	const auto seed = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	fmt::print("[main] seed: {}\n", seed);
	
	tgen::PerlinGenerator pg;
	auto points = pg.generateTerrain(width, height, seed, octaves, amplitude, frequency);
	
	tgen::Mesher mr;
	tgen::Mesh m = mr.triangulate(points);

	std::string filename_path = fmt::format("{}{}.{}",conf["data.path"], seed, conf["data.extension"]);
	
	std::ofstream out(filename_path);
	fmt::print("[main] {} created\n", filename_path);

	CGAL::IO::write_OBJ(out, m);
	
	conf.close();
	out.close();

	return 0;
}