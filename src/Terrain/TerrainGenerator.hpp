#include "../terrain_generation.hpp"
#include "../NoiseGenerator/NoiseGenerator.hpp"
#include "../Mesher/Mesher.hpp"
#include "../Utility/Config.hpp"
#include "./Terrain.hpp"
#include "./Biome.hpp"

#include <fstream>
#include <fmt/format.h>
#include <chrono>
#include <map>


#include <CGAL/IO/OBJ.h>

class tgen::TerrainGenerator {

	tgen::Terrain terrain;

	Biome::BiomeType assignBiomeType(FT humidityValue, FT temperatureValue);

	std::tuple<FT, FT, FT> getBiomeParam(Biome::BiomeType biomeType);

	bool isInBound(Point_2 index, int sizeX, int sizeY);

public:
	TerrainGenerator() {}
	Terrain generateTerrain(unsigned int seed);

	Terrain getTerrain(){
		return this->terrain;
	}
	std::map<std::string, Matrix<FT>> generateMaps(int width, int height, unsigned int seed);
	std::map<std::string, Matrix<FT>> generateHumNTemp(int width, int height, unsigned int seed);
	std::string getBiomeName(Biome::BiomeType biomeType);

};