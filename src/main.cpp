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