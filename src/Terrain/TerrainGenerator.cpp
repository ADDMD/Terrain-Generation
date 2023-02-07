#include "./TerrainGenerator.hpp"

tgen::TerrainGenerator::TerrainGenerator(){}

tgen::Terrain tgen::TerrainGenerator::generateTerrain(unsigned int seed){
	Config conf("../config.yaml");

	std::string w = conf["width"];
	std::string h = conf["height"];
	// amplitude
	std::string ac = conf["continentalness.amplitude"];
	std::string apv = conf["pickNvalley.amplitude"];
	std::string ae = conf["erosion.amplitude"];
	// octaves
	std::string oc = conf["continentalness.octaves"];
	std::string opv = conf["pickNvalley.octaves"];
	std::string oe = conf["erosion.octaves"];
	std::string ot = conf["temperature.octaves"];
	std::string oh = conf["humidity.octaves"];
	// frequency
	std::string fc = conf["continentalness.frequency"];
	std::string fpv = conf["pickNvalley.frequency"];
	std::string fe = conf["erosion.frequency"];
	std::string ft = conf["temperature.frequency"];
	std::string fh = conf["humidity.frequency"];
	// noises
	std::string c = conf["continentalness.noise"];
	std::string pv = conf["pickNvalley.noise"];
	std::string e = conf["erosion.noise"];
	std::string t = conf["temperature.noise"];
	std::string hu = conf["humidity.noise"];

	std::string el = conf["pickNvalley.elevation"];


	// if(w == "" || h == "") return nullptr;

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

	tgen::NoiseGenerator continentalnessGen(noise_continentalness, seed, cOctaves, cAmplitude, cFrequency);
	tgen::NoiseGenerator pickNValleyGen(noise_pickNvalley, seed, pvOctaves, pvAmplitude, pvFrequency);
	tgen::NoiseGenerator erosionGen(noise_erosion, seed, eOctaves, eAmplitude, eFrequency);
	tgen::NoiseGenerator temperatureGen(noise_temperature, seed, tOctaves, 1, tFrequency);
	tgen::NoiseGenerator humidityGen(noise_humidity, seed, hOctaves, 1, hFrequency);
	auto continentalness = continentalnessGen.generateMap(width, height, 1);
	auto temperature = temperatureGen.generateMap(width, height, 1);
	auto humidity = humidityGen.generateMap(width, height, 1);
	auto pickNValley = pickNValleyGen.generateMap(width, height, elevation);
	auto erosion = erosionGen.generateMap(width, height, 1);

	tgen::FT** map;

	map = new tgen::FT*[width];
	for(int i = 0; i < width; i++){
		map[i] = new tgen::FT[height];
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
	mr.triangulate(map, width, height);

	// il refine allunga i tempi (circa 100s in più per una 100x100)
	if(std::stoi(conf["refine"]) == 1)
		mr.refine(); 

	mr.coloring();

	tgen::Mesh mesh = *mr.getMesh();

	this->terrain = Terrain(mesh, map, humidity, temperature);
	return this->terrain;
}

tgen::Terrain tgen::TerrainGenerator::getTerrain(){
	return this->terrain;
}