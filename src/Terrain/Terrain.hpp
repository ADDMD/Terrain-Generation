#include "../terrain_generation.hpp"
#include "../Utility/OBJ.h"

class tgen::Terrain{

	Mesh mesh;

	Matrix<FT> terrainMap;
	Matrix<FT> humidity;
	Matrix<FT> temperature;

public:
	Terrain();
	Terrain(Mesh mesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature);

	Mesh getMesh();
	FT getHeigth(int x, int y);
	Matrix<FT> getHumidity();
	FT getHumidity(int x, int y);
	Matrix<FT> getTemperature();
	FT getTemperature(int x, int y);

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