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

	int maxBiomeSize = std::stoi(conf["biome.size"]);
	auto biomes = computeBiomes(map, humidity, temperature, maxBiomeSize);

	std::map<Point_2, Biome::BiomeType> centroids2biomeType;
	for(auto biome : biomes) {
		centroids2biomeType.insert({biome.getCentroid(), biome.getType()});
	}

	for(auto biome : biomes) {
		for(auto point : biome.getPoints()) {
			int i = point.x();
			int j = point.y();

			auto pointParam = getBiomeParam(biome.getType());
			
			FT currentHeight = continentalness[i][j] * std::get<0>(pointParam) 
				+ pickNValley[i][j] * std::get<1>(pointParam) 
				- erosion[i][j] * std::get<2>(pointParam);

			for(auto centroidPair : centroids2biomeType) {
				auto centroid = centroidPair.first;
				auto centroidParams = getBiomeParam(centroidPair.second);

				FT centroidHeight = continentalness[centroid.x()][centroid.y()] * std::get<0>(centroidParams) 
					+ pickNValley[centroid.x()][centroid.y()] * std::get<1>(centroidParams) 
					- erosion[centroid.x()][centroid.y()] * std::get<2>(centroidParams);

				double param =  1/(1 + std::exp(- CGAL::squared_distance(point, centroid)));
				currentHeight = lerp(currentHeight, centroidHeight, param);
			}

			map[i][j] = currentHeight;
		}
	}


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

bool tgen::TerrainGenerator::isInBound(Point_2 index, int sizeX, int sizeY) {
	return 0 <= index.x() && index.x() < sizeX 
		&& 0 <= index.y() && index.y() < sizeY;
}


void tgen::TerrainGenerator::recursiveComputeBiomes(std::set<Point_2>& visited,
	std::set<Point_2>& result, Point_2 index, Biome::BiomeType biomeType, Matrix<Biome::BiomeType>& biomeMap, int maxBiomeSize) {
	
	// controllo che il punto sia all'interno della mappa
	// che il tipo del bioma sia lo stesso del punto da cui proviene
	// e che non sia già stato visitato
	if(!isInBound(index, biomeMap.size(), biomeMap[0].size())) 
		return;
	if(biomeType != biomeMap[index.x()][index.y()])
		return;
	if(visited.find(index) != visited.end())
		return;

	if (result.size() >= 10000)
		return;

	visited.insert(index);
	result.insert(index);

	// calcolo della possibili vie (quelle adiacenti) sulle quali lanciare la funzione 
	auto up = Point_2(index.x(), index.y() - 1);
	auto down = Point_2(index.x(), index.y() + 1);
	auto left = Point_2(index.x() - 1, index.y());
	auto right = Point_2(index.x() + 1, index.y());

	// chiamate ricorsive sulle vie calcolate prima
	recursiveComputeBiomes(visited, result, up, biomeType, biomeMap, maxBiomeSize);
	recursiveComputeBiomes(visited, result, down, biomeType, biomeMap, maxBiomeSize);
	recursiveComputeBiomes(visited, result, left, biomeType, biomeMap, maxBiomeSize);
	recursiveComputeBiomes(visited, result, right, biomeType, biomeMap, maxBiomeSize);
}


std::vector<tgen::Biome> tgen::TerrainGenerator::computeBiomes(Matrix<FT>& terrainMap, 
	Matrix<FT>& humidity, Matrix<FT>& temperature, int maxBiomeSize) {


	Matrix<Biome::BiomeType> biomeMap = generateMatrix<Biome::BiomeType>(terrainMap.size(), terrainMap[0].size());

	for(int i = 0; i < terrainMap.size(); i++) {
		for(int j = 0; j < terrainMap[0].size(); j++) {
			biomeMap[i][j] = assignBiomeType(humidity[i][j], temperature[i][j]);
		}
	}

	std::set<Point_2> visited;
	std::vector<Biome> biomes;
	for(int i = 0; i < biomeMap.size(); i++) {
		for(int j = 0; j < biomeMap[0].size(); j++) {

			auto index = Point_2(i, j);
			if(visited.find(index) != visited.end()) continue;

			std::set<Point_2> biome;

			recursiveComputeBiomes(visited, biome, index, biomeMap[i][j], biomeMap, maxBiomeSize);
			biomes.push_back(Biome(biome, biomeMap[i][j]));

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

std::tuple<tgen::FT, tgen::FT, tgen::FT> tgen::TerrainGenerator::getBiomeParam(Biome::BiomeType biomeType) {
	std::tuple<FT, FT, FT> result(1, 1, 1);

	switch (biomeType) {
	case Biome::Mountains:
		result = std::tuple<FT, FT, FT>(1, 3, .5); 
		break;
	case Biome::Plains:
		result = std::tuple<FT, FT, FT>(1, .5, 1); 
		break;
	case Biome::Hills:
		result = std::tuple<FT, FT, FT>(1, 1.5, 1.5); 
		break;
	case Biome::Snowy:
		result = std::tuple<FT, FT, FT>(2, 1, 2); 
		break;
	}
	return result;
}






