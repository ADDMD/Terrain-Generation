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

	std::vector<Biome> computeBiomes(Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature);
	std::set<Point_2> recursiveComputeBiomes(std::set<Point_2> visited,
		std::set<Point_2> result, Point_2 index, Biome::BiomeType biomeType,
		Matrix<Biome::BiomeType> humidity);

	Biome::BiomeType assignBiomeType(FT humidityValue, FT temperatureValue);

public:
	TerrainGenerator() {}
	Terrain generateTerrain(unsigned int seed);

	Terrain getTerrain(){
		return this->terrain;
	}
	std::map<std::string, Matrix<FT>> generateMaps(int width, int height, unsigned int seed);

};