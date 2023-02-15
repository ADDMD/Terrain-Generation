#include "./terrain_generation.hpp"
#include "./NoiseGenerator/NoiseGenerator.hpp"
#include "./Mesher/Mesher.hpp"
#include "./Utility/Config.hpp"
#include "./Terrain/TerrainGenerator.hpp"

#include <fstream>
#include <chrono>
#include <string>

#include <fmt/format.h>

#include <CGAL/IO/OBJ.h>

int main(int argc, char const *argv[])
{

	Config conf("../config.yaml");
	tgen::TGENLogger logger = tgen::TGENLogger("main");
	
	logtg::setLoggingLevel(logtg::getLevel(conf["logging.level"]));

	logger.log(logtg::Level::INFO, "Configuration file opened.");

	std::string w = conf["width"];
	std::string h = conf["height"];
	// amplitude
	std::string ac = conf["cAmplitude"];
	std::string apv = conf["pvAmplitude"];
	std::string ae = conf["eAmplitude"];
	// octaves
	std::string oc = conf["cOctaves"];
	std::string opv = conf["pvOctaves"];
	std::string oe = conf["eOctaves"];
	std::string ot = conf["tOctaves"];
	std::string oh = conf["hOctaves"];
	// frequency
	std::string fc = conf["cFrequency"];
	std::string fpv = conf["pvFrequency"];
	std::string fe = conf["eFrequency"];
	std::string ft = conf["tFrequency"];
	std::string fh = conf["hFrequency"];
	// noises
	std::string c = conf["continentalness"];
	std::string pv = conf["pickNvalley"];
	std::string e = conf["erosion"];
	std::string t = conf["temperature"];
	std::string hu = conf["humidity"];

	std::string el = conf["elevation"];


	if(w == "" || h == "") return -1;

	int width = std::stoi(w);
	int height = std::stoi(h);

	int cAmplitude = std::stoi(ac);
	int pvAmplitude = std::stoi(apv);
	int eAmplitude = std::stoi(ae);

	int cOctaves = std::stoi(oc);
	int pvOctaves = std::stoi(opv);
	int eOctaves = std::stoi(oe);
	int tOctaves = std::stoi(ot);
	int hOctaves = std::stoi(oh);

	double cFrequency = std::stod(fc);
	double pvFrequency = std::stod(fpv);
	double eFrequency = std::stod(fe);
	double tFrequency = std::stod(ft);
	double hFrequency = std::stod(fh);

	int noise_continentalness = std::stoi(c);
	int noise_pickNvalley = std::stoi(pv);
	int noise_erosion = std::stoi(e);
	int noise_temperature = std::stoi(t);
	int noise_humidity = std::stoi(hu);

	double elevation = std::stod(el);

	fmt::print("{}\n", cAmplitude);
	fmt::print("{}\n", cOctaves);
	fmt::print("{}\n", cFrequency);
	fmt::print("{}\n", noise_continentalness);

	const auto seed = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	fmt::print("Seed: {}\n", seed);

	tgen::NoiseGenerator continentalnessGen(noise_continentalness, seed, cOctaves, cAmplitude, cFrequency);
	tgen::NoiseGenerator pickNValleyGen(noise_pickNvalley, seed, pvOctaves, pvAmplitude, pvFrequency);
	tgen::NoiseGenerator erosionGen(noise_erosion, seed, eOctaves, eAmplitude, eFrequency);

	tgen::NoiseGenerator humidityGen(noise_humidity, seed, hOctaves, 1, hFrequency);
	tgen::NoiseGenerator temperatureGen(noise_temperature, seed, tOctaves, 1, tFrequency);

	auto continentalness = continentalnessGen.generateMap(width, height, 1);
	auto pickNValley = pickNValleyGen.generateMap(width, height, elevation);
	auto erosion = erosionGen.generateMap(width, height, 1);
	auto humidity = humidityGen.generateMap(width, height, 1);
	auto temperature = temperatureGen.generateMap(width, height, 1);

	auto map = tgen::generateMatrix<tgen::FT>(width, height);
	for(int i = 0; i < width; i++){
	  for(int j = 0; j < height; j++){
	    map[i][j] = continentalness[i][j] + pickNValley[i][j] - erosion[i][j];
	  }
	}
	tgen::Mesher mr;
	mr.triangulate(map, humidity, temperature);
	// il refine allunga i tempi (circa 100s in più per una 100x100)if(std::stoi(conf["refine"]) == 1)
	// mr.refine(); 
	mr.coloring();
	tgen::Mesh mesh = *mr.getMesh();
	std::string ext = conf["data.extension"];
	std::string filename_path = fmt::format("{}{}.{}",conf["data.path"], seed, ext);
	std::ofstream out(filename_path);
	logger.log(logtg::Level::INFO, fmt::format("Created file \"{}\".", filename_path));
	if(ext == "ply")
	  CGAL::IO::write_PLY(out, mesh);           // formato .ply
	else if (ext == "obj")
	  CGAL::IO::write_OBJ(out, mesh);           // formato .obj
	else if(ext == "off")
	  out << mesh;                          // formato .off
	logger.log(logtg::Level::INFO, fmt::format("Mesh saved in \"{}\".", filename_path));
	conf.close();
	out.close();
	return 0;
}