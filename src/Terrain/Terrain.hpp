#include "../terrain_generation.hpp"
#include "../Utility/OBJ.h"
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Surface_mesh_parameterization/parameterize.h>

class tgen::Terrain{

	Mesh terrainMesh;
	Mesh treeMesh;
	std::string name;

	Matrix<FT> terrainMap;
	Matrix<FT> humidity;
	Matrix<FT> temperature;

public:
	Terrain();
	Terrain(std::string name, Mesh terrainMesh, Mesh treeMesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature);

	Mesh getTerrainMesh(){
		return this->terrainMesh;
	}

	Mesh getTreeMesh(){
		return this->treeMesh;
	}

	FT getHeigth(int x, int y){
		return this->terrainMap[x][y];
	}

	Matrix<FT> getHumidity(){
		return this->humidity;
	}

	FT getHumidity(int x, int y){
		return this->humidity[x][y];
	}

	Matrix<FT> getTemperature(){
		return this->temperature;
	}

	FT getTemperature(int x, int y){
		return this->temperature[x][y];
	}

	/** Associate (u,v) coordinates to mesh vertices 
	 * 
	 * This function create a property map to associate (u,v) texture coordinates to vertices.
	 * (@todo: modify those associations:)
	 * - U coordinate is associated to temperature 
	 * - V coordinate is associated to humidity
	 */
	void texturing();

	void save(std::string path);

	/** Salva la mesh in un file .obj con material file associato
	 * 
	 * @param path, percorso del file .obj con cui si salva il terreno
	 * @param materialFileName, nome del .mtl file che si associa al .obj
	 
	 * N.B. Il file .mtl deve essere nella stessa directory del file .obj associato,
	 * 		così come l'eventuale texture (in formato immagine) a cui si riferisce il materiale
	 */
	void saveWithTexture(std::string path, std::string materialFileName);
};