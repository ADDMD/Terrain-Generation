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

	const auto seed = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();

	tgen::TerrainGenerator tg;
	tgen::Terrain terrain = tg.generateTerrain(seed);

	std::string ext = conf["data.extension"];

	std::string filename_path = fmt::format("{}{}.{}",conf["data.path"], seed, ext);

	logger.log(logtg::Level::INFO, fmt::format("Created file \"{}\".", filename_path));
	terrain.save(filename_path);
	logger.log(logtg::Level::INFO, fmt::format("Mesh saved in \"{}\".", filename_path));
	
	conf.close();

	return 0;
}