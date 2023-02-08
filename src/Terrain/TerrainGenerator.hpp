#include "../terrain_generation.hpp"
#include "../NoiseGenerator/NoiseGenerator.hpp"
#include "../Mesher/Mesher.hpp"
#include "../Utility/Config.hpp"
#include "./Terrain.hpp"

#include <fstream>
#include <fmt/format.h>
#include <chrono>
#include <map>

#include <CGAL/Named_function_parameters.h>
#include <CGAL/IO/OBJ.h>

class tgen::TerrainGenerator {

	tgen::Terrain terrain;

public:
	TerrainGenerator();
	tgen::Terrain generateTerrain(unsigned int seed);
	tgen::Terrain getTerrain();
	std::map<std::string, tgen::FT**> generateMaps(int width, int height, unsigned int seed);
};