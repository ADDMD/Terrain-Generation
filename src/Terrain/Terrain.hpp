#include "../terrain_generation.hpp"
#include "../Utility/OBJ.h"

class tgen::Terrain{

	tgen::Mesh mesh;

	tgen::FT** terrainMap;
	tgen::FT** humidity;
	tgen::FT** temperature;

public:
	Terrain();
	Terrain(tgen::Mesh mesh, tgen::FT** terrainMap, tgen::FT** humidity, tgen::FT** temperature);

	tgen::Mesh getMesh();
	tgen::FT getHeigth(int x, int y);
	tgen::FT** getHumidity();
	tgen::FT getHumidity(int x, int y);
	tgen::FT** getTemperature();
	tgen::FT getTemperature(int x, int y);

	/** Associate (u,v) coordinates to mesh vertices 
	 * 
	 * This function create a property map to associate (u,v) texture coordinates to vertices.
	 * (@todo: modify those associations:)
	 * - U coordinate is associated to temperature 
	 * - V coordinate is associated to humidity
	 */
	void texturing();

	void save(std::string path);
};