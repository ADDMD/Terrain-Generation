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

	tgen::Terrain terrain;

public:
	TerrainGenerator() {}
	Terrain generateTerrain(unsigned int seed);
	Terrain getTerrain();
	std::map<std::string, Matrix<FT>> generateMaps(int width, int height, unsigned int seed);
};