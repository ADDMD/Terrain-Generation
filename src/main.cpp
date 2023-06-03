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

	std::string filename_path = fmt::format("{}{}",conf["data.path"], seed);

	// Se la mesh deve avere una texture, la mesh deve essere salvata in un file obj
	// a cui associo un material file
	int with_texture = std::stoi(conf["data.withTexture"]);
	if(with_texture){
		std::string mtl_path = conf["data.mtl"]; // file material da associare alla mesh
		filename_path.append(".obj"); // se voglio usare i material file, la mesh deve essere salvata come un obj
		logger.log(logtg::Level::INFO, fmt::format("Created obj file with mtl at \"{}\".", filename_path));
		// salvo mesh con texture
		terrain.saveWithTexture(filename_path, mtl_path);
		logger.log(logtg::Level::INFO, fmt::format("Mesh saved with texture \"{}\" in \"{}\".", mtl_path, filename_path));
		return 0;
	}

	std::string ext = conf["data.extension"];

	filename_path.append(fmt::format(".{}", ext));

	logger.log(logtg::Level::INFO, fmt::format("Created file \"{}\".", filename_path));
	terrain.save(filename_path);
	logger.log(logtg::Level::INFO, fmt::format("Mesh saved in \"{}\".", filename_path));
	
	conf.close();

	return 0;
}