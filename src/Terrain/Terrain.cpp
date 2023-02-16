#include "./Terrain.hpp"

tgen::Terrain::Terrain(){}
tgen::Terrain::Terrain(tgen::Mesh mesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature){
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

tgen::Matrix<tgen::FT> tgen::Terrain::getHumidity(){
	return this->humidity;
}

tgen::FT tgen::Terrain::getHumidity(int x, int y){
	return this->humidity[x][y];
}

tgen::Matrix<tgen::FT> tgen::Terrain::getTemperature(){
	return this->temperature;
}

tgen::FT tgen::Terrain::getTemperature(int x, int y){
	return this->temperature[x][y];
}

void tgen::Terrain::texturing(){
	// Create the property map, called "v:uv", for vertices' (u,v) coordinates [vertex v -> (u,v)]
	Mesh::Property_map<vertex_descriptor, Point_2> uvmap = this->mesh.add_property_map<vertex_descriptor, Point_2>("v:uv").first;

	// Associate (u,v) coordinates to each mesh's vertex
	for(vertex_descriptor v : vertices(this->mesh)){
		// Get vertex's 3D point
		Point_3 p = this->mesh.point(v);
		// u coordinate
		tgen::FT vertexTemperature = getTemperature(p.x(), p.y());
		// v coordinate
		tgen::FT vertexHumidity = getHumidity(p.x(), p.y());
		
		// Update vertex's (u,v) coordinate
		uvmap[v] = Point_2(vertexTemperature, vertexHumidity);
		// put(uvmap, v, Point_2(vertexTemperature, vertexHumidity));
	}
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

void tgen::Terrain::saveWithTexture(std::string filePath, std::string materialFileName){
	std::ofstream out(filePath);
	write_OBJ_MTL(out, mesh, materialFileName, CGAL::parameters::vertex_texture_map(mesh.property_map<vertex_descriptor, Point_2>("v:uv").first));
}