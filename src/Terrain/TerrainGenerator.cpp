#include "./TerrainGenerator.hpp"
#include "../Utility/ImageWriter/ImageWriter.cpp"
#include "../Utility/Json.hpp"
#include <fstream>

tgen::Terrain tgen::TerrainGenerator::generateTerrain(unsigned int seed){
	Config conf("../config.yaml");
	std::string w = conf["width"];
	std::string h = conf["height"];

	int width = std::stoi(w);
	int height = std::stoi(h);


	std::map<std::string, Matrix<FT>> biomeMaps = generateMaps(width, height, seed);
	std::map<std::string, Matrix<FT>> humNtemp = generateHumNTemp(width, height, seed);
	auto map = generateMatrix<FT>(width, height);

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			std::string biomeName = getBiomeName(assignBiomeType(humNtemp["humidity"][i][j], humNtemp["temperature"][i][j]));
			map[i][j] = biomeMaps[biomeName][i][j];
		}
	}

	tgen::Mesher mr;
	mr.triangulate(map);

	// // il refine allunga i tempi (circa 100s in più per una 100x100)
	// if(std::stoi(conf["refine"]) == 1)
	// 	mr.refine(); 

	mr.coloring();
	// mr.texturing();

	tgen::Mesh mesh = *mr.getMesh();

	this->terrain = Terrain(mesh, map, humNtemp["humidity"], humNtemp["temperature"]);
	return this->terrain;
}

std::map<std::string, tgen::Matrix<tgen::FT>> tgen::TerrainGenerator::generateHumNTemp(int width, int height, unsigned int seed){
	std::ifstream f("../biomes.json");
	nlohmann::json data = nlohmann::json::parse(f);
	std::map<std::string, Matrix<FT>> maps;

	for(auto type: data["type"]){
		auto mapName = data["maps"];
		// amplitude
		int noise = data[type][mapName][0];
		// octaves
		int amplitude = data[type][mapName][1];
		// frequency
		int octaves = data[type][mapName][2];
		// noises
		double frequency = data[type][mapName][3];
		// elevation
		double elevation = data[type][mapName][4];

		std::hash<std::string> intHash;

		tgen::NoiseGenerator gen(noise, seed xor intHash(mapName) xor intHash(type), octaves, amplitude, frequency);
		auto map = gen.generateMap(width, height, elevation);

		fmt::print("Creato: {}\n", type);
		maps.insert({type, map});
	}
	return maps;
}


std::map<std::string, tgen::Matrix<tgen::FT>> tgen::TerrainGenerator::generateMaps(int width, int height, unsigned int seed){
	std::ifstream f("../biomeTypes.json");
	nlohmann::json data = nlohmann::json::parse(f);
	std::map<std::string, Matrix<FT>> maps;

	for(auto type: data["type"]){
		std::map<std::string, Matrix<FT>> generativeMaps;
		for(auto mapName : data["maps"]){

			// amplitude
			int noise = data[type][mapName][0];
			// octaves
			int amplitude = data[type][mapName][1];
			// frequency
			int octaves = data[type][mapName][2];
			// noises
			double frequency = data[type][mapName][3];
			// elevation
			double elevation = data[type][mapName][4];

			std::hash<std::string> intHash;

			tgen::NoiseGenerator gen(noise, seed xor intHash(mapName) xor intHash(type), octaves, amplitude, frequency);
			auto map = gen.generateMap(width, height, elevation);

			generativeMaps.insert({mapName, map});
		}

		auto typeMap = generateMatrix<FT>(width, height);
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				typeMap[i][j] = generativeMaps["continentalness"][i][j] + generativeMaps["picknvalley"][i][j] - generativeMaps["erosion"][i][j];
			}
		}
		fmt::print("Creato: {}\n", type);
		maps.insert({type, typeMap});
	}
	return maps;
}

bool tgen::TerrainGenerator::isInBound(Point_2 index, int sizeX, int sizeY) {
	return 0 <= index.x() && index.x() < sizeX 
		&& 0 <= index.y() && index.y() < sizeY;
}

tgen::Biome::BiomeType tgen::TerrainGenerator::assignBiomeType(FT humidityValue, FT temperatureValue) {
	Biome::BiomeType result;

	if(humidityValue < .5 && temperatureValue < .5) {
		result = Biome::BiomeType::Mountains;
	} else if(humidityValue >= .5 && temperatureValue < .5) {
		result = Biome::BiomeType::Desert;
	} else if(humidityValue < .5 && temperatureValue >= .5) {
		result = Biome::BiomeType::Hills;
	} else {
		result = Biome::BiomeType::Plains;
	}

	return result;
}
std::string tgen::TerrainGenerator::getBiomeName(Biome::BiomeType biomeType){
	std::string result;
	switch (biomeType) {
	case Biome::Mountains:
		result = "montagne"; 
		break;
	case Biome::Desert:
		result = "deserto"; 
		break;
	case Biome::Hills:
		result = "colline"; 
		break;
	case Biome::Plains:
		result = "pianura"; 
		break;
	}
	return result;
}

std::tuple<tgen::FT, tgen::FT, tgen::FT> tgen::TerrainGenerator::getBiomeParam(Biome::BiomeType biomeType) {
	std::tuple<FT, FT, FT> result(1, 1, 1);

	switch (biomeType) {
	case Biome::Mountains:
		result = std::tuple<FT, FT, FT>(1, 5, 5); 
		break;
	case Biome::Desert:
		result = std::tuple<FT, FT, FT>(1, 1, 5); 
		break;
	case Biome::Hills:
		result = std::tuple<FT, FT, FT>(1, 5, 1); 
		break;
	case Biome::Plains:
		result = std::tuple<FT, FT, FT>(1, 1, 1); 
		break;
	}
	return result;
}






