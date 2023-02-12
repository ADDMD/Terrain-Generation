#include "./TerrainGenerator.hpp"
#include "../Utility/ImageWriter/ImageWriter.cpp"


tgen::Terrain tgen::TerrainGenerator::generateTerrain(unsigned int seed){
	Config conf("../config.yaml");
	std::string w = conf["width"];
	std::string h = conf["height"];

	// if(w == "" || h == "") return nullptr;

	int width = std::stoi(w);
	int height = std::stoi(h);

	std::map<std::string, Matrix<FT>>  maps = generateMaps(width, height, seed);
	auto map = generateMatrix<FT>(width, height);

	auto continentalness = maps["continentalness"];
	auto pickNValley = maps["pickNvalley"];
	auto erosion = maps["erosion"];
	auto humidity = maps["humidity"];
	auto temperature = maps["temperature"];

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			map[i][j] = continentalness[i][j] + pickNValley[i][j] - erosion[i][j];
		}
	}
	computeBiomes(map, humidity, temperature);

	tgen::Mesher mr;
	mr.triangulate(map);

	// il refine allunga i tempi (circa 100s in più per una 100x100)
	if(std::stoi(conf["refine"]) == 1)
		mr.refine(); 

	mr.coloring();
	// mr.texturing();

	tgen::Mesh mesh = *mr.getMesh();

	conf.close();

	this->terrain = Terrain(mesh, map, humidity, temperature);
	return this->terrain;
}



std::map<std::string, tgen::Matrix<tgen::FT>> tgen::TerrainGenerator::generateMaps(int width, int height, unsigned int seed){
	Config conf("../config.yaml");
	std::map<std::string, Matrix<FT>> maps;
	std::vector<std::string> mapnames({"continentalness", "pickNvalley", "erosion", "humidity", "temperature"});

	for(auto name : mapnames){
		// amplitude
		std::string ampl = conf[fmt::format("{}.amplitude", name)];
		// octaves
		std::string oct = conf[fmt::format("{}.octaves", name)];
		// frequency
		std::string freq = conf[fmt::format("{}.frequency", name)];
		// noises
		std::string noi = conf[fmt::format("{}.noise", name)];
		// elevation
		std::string elev = conf[fmt::format("{}.elevation", name)];

		int amplitude = std::stoi(ampl);
		int octaves = std::stoi(oct);
		double frequency = std::stod(freq);
		int noise = std::stoi(noi);
		double elevation = std::stod(elev);

		std::hash<std::string> intHash;

		tgen::NoiseGenerator gen(noise, seed xor intHash(name), octaves, amplitude, frequency);
		auto map = gen.generateMap(width, height, elevation);

		maps.insert({name, map});

		// unsigned char image[height][width][BYTES_PER_PIXEL];

		// std::string fileName = fmt::format("{}.bmp", name); 
	    // char* imageFileName;
	    // std::strcpy(imageFileName, fileName.c_str());

	    // int i, j;
	    // for (i = 0; i < width; i++) {
	    //     for (j = 0; j < height; j++) {
	    //    		double value = (map[i][j] / amplitude) * 255;
	    //         image[i][j][2] = (unsigned char) (value);             ///red
	    //         image[i][j][1] = (unsigned char) (value);              ///green
	    //         image[i][j][0] = (unsigned char) (value); ///blue
	    //     }
	    // }

	    // generateBitmapImage((unsigned char*) image, height, width, imageFileName);
	}

	conf.close();
	return maps;
}

bool inBound(int index, int size) {
	return 0 <= index && index < size ;
}


std::set<tgen::Point_2> tgen::TerrainGenerator::recursiveComputeBiomes(std::set<Point_2> visited,
	std::set<Point_2> result, Point_2 index, Biome::BiomeType biomeType, Matrix<Biome::BiomeType> biomeMap) {

	if(!(inBound((int)index.x(), biomeMap.size()) && inBound((int)index.y(), biomeMap[0].size())))
		return result;

	fmt::print("In bound\n");

	if(!visited.insert(index).second) 
		return result;
	fmt::print("Not visited\n");

	if (biomeType != assignBiomeType(biomeMap[(int)index.x()][(int)index.y()], biomeMap[(int)index.x()][(int)index.y()])) 
		return result;

	result.insert(index);

	auto up = Point_2((int)index.x(), (int)index.y() - 1, 0);
	auto down = Point_2((int)index.x(), (int)index.y() + 1, 0);
	auto left = Point_2((int)index.x() - 1, (int)index.y(), 0);
	auto right = Point_2((int)index.x() + 1, (int)index.y(), 0);


	auto upResult = recursiveComputeBiomes(visited, result, up, biomeType, biomeMap);
	result.insert(upResult.begin(), upResult.end());

	auto downResult = recursiveComputeBiomes(visited, result, down, biomeType, biomeMap);
	result.insert(downResult.begin(), downResult.end());

	auto leftResult = recursiveComputeBiomes(visited, result, left, biomeType, biomeMap);
	result.insert(leftResult.begin(), leftResult.end());

	auto rightResult = recursiveComputeBiomes(visited, result, right, biomeType, biomeMap);
	result.insert(rightResult.begin(), rightResult.end());

	return result;
}


std::vector<tgen::Biome> tgen::TerrainGenerator::computeBiomes(Matrix<FT> terrainMap, 
	Matrix<FT> humidity, Matrix<FT> temperature) {

	std::vector<Biome> biomes;
	std::set<Point_2> visited;

	Matrix<Biome::BiomeType> biomeMap = generateMatrix<Biome::BiomeType>(terrainMap.size(), terrainMap[0].size());

	for(int i = 0; i < terrainMap.size(); i++) {
		for(int j = 0; j < terrainMap[0].size(); j++) {
			biomeMap[i][j] = assignBiomeType(humidity[i][j], temperature[i][j]);
			fmt::print("({}, {}) = {} ", i, j, biomeMap[i][j]);
		}
		fmt::print("\n");
	}

	for(int i = 0; i < biomeMap.size(); i++) {
		for(int j = 0; j < biomeMap[0].size(); j++) {

			auto index = Point_2(i, j);
			if(visited.find(index) != visited.end()) continue;

			std::set<Point_2> biome;

			biome = recursiveComputeBiomes(visited, biome, index, biomeMap[i][j], biomeMap);

			visited.insert(biome.begin(), biome.end());

			fmt::print("biome size: {}\n", biome.size());

		}
	}

	return biomes;
}

tgen::Biome::BiomeType tgen::TerrainGenerator::assignBiomeType(FT humidityValue, FT temperatureValue) {
	Biome::BiomeType result;

	if(humidityValue < .5 && temperatureValue < .5) {
		result = Biome::BiomeType::Mountains;
	} else if(humidityValue >= .5 && temperatureValue < .5) {
		result = Biome::BiomeType::Snowy;
	} else if(humidityValue < .5 && temperatureValue >= .5) {
		result = Biome::BiomeType::Hills;
	} else {
		result = Biome::BiomeType::Plains;
	}

	return result;
}






