#include "./Terrain.hpp"
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#define _USE_MATH_DEFINES

tgen::Terrain::Terrain(){}
tgen::Terrain::Terrain(std::string name, tgen::Mesh terrainMesh, tgen::Mesh treeMesh, Matrix<FT> terrainMap, Matrix<FT> humidity, Matrix<FT> temperature){
	this->name = name;
	this->terrainMesh = terrainMesh;
	this->treeMesh = treeMesh;
	this->terrainMap = terrainMap;
	this->humidity = humidity;
	this->temperature = temperature;
}

void tgen::Terrain::texturing(){
	// Create the property map, called "v:uv", for vertices' (u,v) coordinates [vertex v -> (u,v)]
	Mesh::Property_map<vertex_descriptor, Point_2> uvmap = this->terrainMesh.add_property_map<vertex_descriptor, Point_2>("v:uv").first;

	// Associate (u,v) coordinates to each terrainMesh's vertex
	for(vertex_descriptor vert : vertices(this->terrainMesh)){
		// Get vertex's 3D point
		Point_3 p = this->terrainMesh.point(vert);
		auto normal = CGAL::Polygon_mesh_processing::compute_vertex_normal(vert, this->terrainMesh);


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
		CGAL::IO::write_PLY(out, this->terrainMesh);			// formato .ply
	else if (ext == "obj")
		CGAL::IO::write_OBJ(out, this->terrainMesh);			// formato .obj
	else if(ext == "off")
		out << this->terrainMesh; 							// formato .off

	out.close();	
}

void tgen::Terrain::saveWithTexture(std::string filePath, std::string materialFileName){
	std::ofstream terrainOut(filePath);
	int position=filePath.find_last_of(".");
	std::ofstream treeOut(fmt::format("{}_tree.{}", filePath.substr(0, position-1), filePath.substr(position+1)));
	write_OBJ_MTL(terrainOut, this->terrainMesh, materialFileName, CGAL::parameters::vertex_texture_map(this->terrainMesh.property_map<vertex_descriptor, Point_2>("v:uv").first));
	CGAL::IO::write_OBJ(treeOut, this->treeMesh);
}