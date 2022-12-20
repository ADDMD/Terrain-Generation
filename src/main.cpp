#include "./terrain_generation.hpp"
#include "./NoiseGenerator/NoiseGenerator.hpp"
#include "./Mesher/Mesher.hpp"
#include "./Utility/Config.hpp"

#include <fstream>
#include <chrono>
#include <string>

#include <fmt/format.h>

#include <CGAL/Named_function_parameters.h>
#include <CGAL/IO/OBJ.h>



int main(int argc, char const *argv[])
{
	tgen::TGENLogger logger = tgen::TGENLogger("main");
	
	Config conf("../config.yaml");
	log::setLoggingLevel(log::getLevel(conf["logging.level"]));

	logger.log(log::Level::INFO, "Configuration file opened.");

	std::string w = conf["width"];
	std::string h = conf["height"];
	std::string a = conf["amplitude"];
	std::string o = conf["octaves"];
	std::string f = conf["frequency"];
	std::string n = conf["noise"];


	if(w == "" || h == "") return -1;

	int width = std::stoi(w);
	int height = std::stoi(h);
	int amplitude = std::stoi(a);
	int octaves = std::stoi(o);
	double frequency = std::stod(f);
	int noise = std::stoi(n);

	const auto seed = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	logger.log(log::Level::INFO, 
		fmt::format("Seed = {}; Width = {}; Height = {}; Amplitude = {}; Frequency = {}; Noise = {}.",
		seed, width, height, amplitude, frequency, noise));

	tgen::NoiseGenerator ng(noise, seed, octaves, amplitude, frequency);
	auto points = ng.generatePointsMatrix(width, height);

	tgen::Mesher mr;
	mr.triangulate(points, width, height);
	// mr.triangulate(points);
	
	// il refine allunga i tempi (circa 100s in più per una 100x100)
	if(std::stoi(conf["refine"]) == 1)
		mr.refine(); 

	tgen::Mesh mesh = *mr.getMesh();

	std::string ext = conf["data.extension"];

	std::string filename_path = fmt::format("{}{}.{}",conf["data.path"], seed, ext);
	std::ofstream out(filename_path);

	logger.log(log::Level::INFO, fmt::format("Created file \"{}\".", filename_path));
	if(ext == "ply")
		CGAL::IO::write_PLY(out, mesh);			// formato .ply
	else if (ext == "obj")
		CGAL::IO::write_OBJ(out, mesh);			// formato .obj
	else if(ext == "off")
		out << mesh; 							// formato .off
	
	logger.log(log::Level::INFO, fmt::format("Mesh saved in \"{}\".", filename_path));
	
	conf.close();
	out.close();

	return 0;
}