#include "./Terrain.hpp"

tgen::Terrain::Terrain(){}
tgen::Terrain::Terrain(tgen::Mesh mesh, tgen::FT** terrainMap, tgen::FT** humidity, tgen::FT** temperature){
	this->mesh = mesh;
	this->terrainMap = terrainMap;
	this->humidity = humidity;
	this->temperature = temperature;
}

tgen::Mesh tgen::Terrain::getMesh(){
	return this->mesh;
}

tgen::FT tgen::Terrain::getHeigth(int x, int y){
	return this->terrainMap[x][y];
}

tgen::FT** tgen::Terrain::getHumidity(){
	return this->humidity;
}

tgen::FT tgen::Terrain::getHumidity(int x, int y){
	return this->humidity[x][y];
}

tgen::FT** tgen::Terrain::getTemperature(){
	return this->temperature;
}

tgen::FT tgen::Terrain::getTemperature(int x, int y){
	return this->temperature[x][y];
}

void tgen::Terrain::save(std::string filePath){
	//store the position of last '.' in the file name
	int position=filePath.find_last_of(".");

	//store the characters after the '.' from the filePath string
	std::string ext = filePath.substr(position+1);

	std::ofstream out(filePath);
	if(ext == "ply")
		CGAL::IO::write_PLY(out, mesh);			// formato .ply
	else if (ext == "obj")
		CGAL::IO::write_OBJ(out, mesh);			// formato .obj
	else if(ext == "off")
		out << mesh; 							// formato .off

	out.close();	
}