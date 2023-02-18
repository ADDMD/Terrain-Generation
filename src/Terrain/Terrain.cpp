#include "./Terrain.hpp"
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#define _USE_MATH_DEFINES

tgen::Terrain::Terrain(){}
tgen::Terrain::Terrain(std::string name, tgen::Mesh mesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature){
	this->name = name;
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
	for(vertex_descriptor vert : vertices(this->mesh)){
		// Get vertex's 3D point
		Point_3 p = this->mesh.point(vert);
		auto normal = CGAL::Polygon_mesh_processing::compute_vertex_normal(vert, mesh);


		// Calcoli effettuati grazie al grandioso contributo di
		// https://math.stackexchange.com/questions/2049179/mapping-from-a-unit-square-to-a-unit-circle-and-more-generally-to-an-ellipse
		
		// u coordinate
		tgen::FT u = getTemperature(p.x(), p.y()) - 0.5;
		// v coordinate
		tgen::FT v = getHumidity(p.x(), p.y()) - 0.5;
		double s = 2 * sqrt(u*u + v*v);
		double radialX = u * ((1-normal.z())*0.95 + 0.05)/ s + .5;
		double radialY = v * ((1-normal.z())*0.95 + 0.05)/ s + .5;
		Point_2 uv = Point_2(radialX, radialY);
		
		// Update vertex's (u,v) coordinate
		uvmap[vert] = uv;
		// put(uvmap, v, Point_2(vertexTemperature, vertexHumidity));
	}
}

void tgen::Terrain::save(std::string filePath){
	//store the position of last '.' in the file name
	int position=filePath.find_last_of(".");

	//store the characters after the '.' from the filePath string
	std::string ext = filePath.substr(position+1);

	std::string final_path = fmt::format("{}{}.{}", filePath.substr(0, position-1), this->name, ext);

	std::ofstream out(final_path);
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