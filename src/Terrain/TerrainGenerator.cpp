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

	auto finalMap = generateMatrix<FT>(width, height);
	auto humidity = humNtemp["humidity"];
	auto temperature = humNtemp["temperature"];

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			double hum = humidity[i][j];
			double tem = temperature[i][j];
			std::vector<BiomeType> biomes = {Mountains, Desert, Hills, Plains};
			for(auto biome: biomes){
				finalMap[i][j] += biomeMaps[getBiomeName(biome)][i][j] * computeDistanceFromBiome(biome, hum, tem);
			}
		}
	}

	tgen::Mesher mr;
	mr.triangulate(finalMap);

	// mr.refine();
	tgen::Mesh mesh = *mr.getMesh();

	std::string path_tree = conf["assets.tree"];
	std::ifstream input(path_tree);
	Mesh treeMesh;

	Mesh treeMapMesh;

	if (!input || !CGAL::IO::read_OBJ(input, treeMesh)) {
		std::cerr << "Errore nella lettura del file obj" << std::endl;
	}

	for(auto v : mesh.vertices()){
			auto normal = CGAL::Polygon_mesh_processing::compute_vertex_normal(v, mesh);

			if (normal.z() >= 0.8){
				
				//generazione di numeri casuali tra 0 e 1
				std::mt19937 generator(std::random_device{}());
				std::uniform_real_distribution<double> distribution(0, 1);
				//generazione numero casuale
				auto num_ran = distribution(generator);

				Point_3 point = mesh.point(v);
				auto x = point.x();
				auto y = point.y();
				double spawningThreshold = 1;
				switch (assignBiomeType(humidity[x][y], temperature[x][y])) {
				case Mountains: 
					if(point.z() > 30) spawningThreshold = 0.97;
					else if(point.z() > 10) spawningThreshold = 0.88;
					break;
				case Hills:
					if(point.z() > 20) spawningThreshold = 0.97;
					else if(point.z() > 10) spawningThreshold = 0.88;
					break;
				case Plains:
					if(point.z() > 10) spawningThreshold = 0.88;
					break;
				case Desert:
					spawningThreshold = 1;
					break;
				}
				if (num_ran > spawningThreshold)
					{
						mr.joinAndTrasformTreeMesh(treeMapMesh, treeMesh, point);
					}
			}
	}

	this->terrain = Terrain("Terreno finale", mesh, treeMapMesh, finalMap, humNtemp["humidity"], humNtemp["temperature"]);
	this->terrain.texturing();
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

	unsigned char image[width][height][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";
    char* mapFileName;
    strcpy(mapFileName, fmt::format("{}{}{}", "../data/", seed, imageFileName).c_str());
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
        	auto temp = maps["temperature"][i][j];
        	auto hum = maps["humidity"][i][j];
        	CGAL::Color c;
        	if(hum < .5 &&  temp < .5) {
				c = mountain;
			} else if(hum >= .5 && temp < .5) {
				c = darkgreen;
			} else if(hum < .5 && temp >= .5) {
				c = grey;
			} else {
				c = grass;
			}

			auto result = c.to_rgb();
			c.set_rgb(result[0], result[1], result[2]);
            image[i][j][2] = c.r(); ///red
            image[i][j][1] = c.g(); ///green
            image[i][j][0] = c.b(); ///blue
        }
    }

    generateBitmapImage((unsigned char*) image, height, width, mapFileName);
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

tgen::TerrainGenerator::BiomeType tgen::TerrainGenerator::assignBiomeType(FT humidityValue, FT temperatureValue) {
	BiomeType result;

	if(humidityValue < .5 && temperatureValue < .5) {
		result = BiomeType::Mountains;
	} else if(humidityValue >= .5 && temperatureValue < .5) {
		result = BiomeType::Hills;
	} else if(humidityValue < .5 && temperatureValue >= .5) {
		result = BiomeType::Desert;
	} else {
		result = BiomeType::Plains;
	}

	return result;
}

tgen::FT tgen::TerrainGenerator::computeDistanceFromBiome(BiomeType biomeType, FT hum, FT temp){
	FT result;
	switch (biomeType) {
	case Mountains:
		result = (hum * hum + temp * temp); 
		break;
	case Desert:
		result = (hum * hum + (temp -1) * (temp - 1)); 
		break;
	case Hills:
		result = ((hum - 1) * (hum - 1) + temp * temp); 
		break;
	case Plains:
		result = ((hum - 1) * (hum - 1) + (temp -1) * (temp - 1)); 
		break;
	}
	return 1 - (result * result * result / 8);
}

std::string tgen::TerrainGenerator::getBiomeName(BiomeType biomeType){
	std::string result;
	switch (biomeType) {
	case Mountains:
		result = "montagne"; 
		break;
	case Desert:
		result = "deserto"; 
		break;
	case Hills:
		result = "colline"; 
		break;
	case Plains:
		result = "pianura"; 
		break;
	}
	return result;
}