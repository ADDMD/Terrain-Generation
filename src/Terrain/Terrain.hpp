#include "../terrain_generation.hpp"

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

	void save(std::string path);
};