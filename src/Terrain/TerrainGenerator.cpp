#include "./TerrainGenerator.hpp"

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
			double h = humidity[i][j];
			double t = temperature[i][j];
			double pvparam, eparam;
			if (h < .33 && t < .33){
				pvparam = 2.5;
				eparam = 2.5;
			}else if(h < .33 && t > .33 && t < .66){
				pvparam = 3.75;
				eparam = 2.5;
			}else if(t < .33 && h > .33 && h < .66){
				pvparam = 2.5;
				eparam = 3.75;
			} else if(h > .33 && h < .66 && t > .33 && t < .66){
				pvparam = 3.75;
				eparam = 3.75;
			} else if(h < .33 && t > .66){
				pvparam = 5;
				eparam = 2.5;
			} else if(h > .66 && t < .33){
				pvparam = 2.5;
				eparam = 5;
			} else if(h > .33 && h < .66 && t > .66){
				pvparam = 5;
				eparam = 3.75;
			}else if(h > .66 && t > .33 && t < .66){
				pvparam = 3.75;
				eparam = 5;
			} else {
				pvparam = 5;
				eparam = 5;
			}
			pvparam = pvparam * t * h;
			eparam = eparam * t * h;
			map[i][j] = continentalness[i][j] + pvparam * pickNValley[i][j] - eparam * erosion[i][j];
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

tgen::Terrain tgen::TerrainGenerator::getTerrain(){
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

		tgen::NoiseGenerator gen(noise, seed, octaves, amplitude, frequency);
		auto map = gen.generateMap(width, height, elevation);

		maps.insert({name, map});
	}

	conf.close();
	return maps;
}