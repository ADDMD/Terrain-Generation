#include "../terrain_generation.hpp"
#include <CGAL/IO/OBJ.h>

class tgen::Terrain{

	Mesh mesh;
	std::string name;

	Matrix<FT> terrainMap;
	Matrix<FT> humidity;
	Matrix<FT> temperature;

public:
	Terrain();
	Terrain(std::string name, Mesh mesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature);

	Mesh getMesh();
	FT getHeigth(int x, int y);
	Matrix<FT> getHumidity();
	FT getHumidity(int x, int y);
	Matrix<FT> getTemperature();
	FT getTemperature(int x, int y);

	void save(std::string path);
};