#include "../terrain_generation.hpp"
#include "../NoiseGenerator/NoiseGenerator.hpp"
#include "../Mesher/Mesher.hpp"
#include "../Utility/Config.hpp"
#include "./Terrain.hpp"

#include <fstream>
#include <fmt/format.h>
#include <chrono>
#include <map>


#include <CGAL/IO/OBJ.h>

class tgen::TerrainGenerator {

	enum BiomeType {
		Mountains = 0,
		Hills,
		Desert,
		Plains
	};
	
	tgen::Terrain terrain;

	BiomeType assignBiomeType(FT humidityValue, FT temperatureValue);


public:
	TerrainGenerator() {}
	Terrain generateTerrain(unsigned int seed);

	Terrain getTerrain(){
		return this->terrain;
	}
	std::map<std::string, Matrix<FT>> generateMaps(int width, int height, unsigned int seed);
	std::map<std::string, Matrix<FT>> generateHumNTemp(int width, int height, unsigned int seed);
	FT computeDistanceFromBiome(BiomeType biomeType, FT hum, FT temp);
	std::string getBiomeName(BiomeType biomeType);

};