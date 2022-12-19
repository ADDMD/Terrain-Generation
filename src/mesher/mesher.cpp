#include "Mesher.hpp"

#include <CGAL/Advancing_front_surface_reconstruction.h>

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/IO/Color.h>

#include <map>

tgen::Mesher::Mesher() {}

void tgen::Mesher::triangulate(std::vector<Point> points) {
	mesh = new Mesh();

	Construct construct(*mesh, points.begin(), points.end());
	CGAL::advancing_front_surface_reconstruction(points.begin(), points.end(), construct);

	printSummary();
}

template <typename T, std::size_t N>
inline std::size_t size(T (&)[N])
{
    return N;
}

/** Metodo che genera una mesh triangolare a partire da una griglia di punti 
 *
 *	Triangolazione:
 *
 *  p0       p1           v0       v1
 *	*--------*            *--------*
 *	|        |   Triang.  |\__  f1 |
 *	|        |     ->     |   \__  |
 *	|        |            | f2   \_|
 *	*--------*            *--------*
 *	p2       p3           v2       v3
 *
 *	I vertici delle facce sono definiti in senso antiorario
 */
void tgen::Mesher::triangulate(Point** points, const int width, const int height) {
	// int width_1 = size(points);
	// int height_1 = *(points + 1) - *points;
	
	// fmt::print("width_1 = {}\n", width_1);
	// fmt::print("height_1 = {}\n", height_1);
	
	mesh = new Mesh();
	std::map<Point, Mesh::vertex_index> pnt2idx;

	// aggiungo tutti i punti alla mesh come vertici
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			Point p = points[x][y];
			pnt2idx.insert({p, mesh->add_vertex(p)});
		}
	}

	// triangolo le facce
	for(int x0 = 0, x1 = 1; x1 < width; x0++, x1++) {
		for(int y0 = 0, y1 = 1; y1 < height; y0++, y1++) {
			
			Point p0 = points[x0][y0];
			Point p1 = points[x1][y0];
			Point p2 = points[x0][y1];
			Point p3 = points[x1][y1];

			Mesh::vertex_index v0 = pnt2idx.find(p0)->second;
			Mesh::vertex_index v1 = pnt2idx.find(p1)->second;
			Mesh::vertex_index v2 = pnt2idx.find(p2)->second;
			Mesh::vertex_index v3 = pnt2idx.find(p3)->second;

			// counterclockwise orientation
			// mesh->add_face(v0, v3, v1); // f1
			// mesh->add_face(v0, v2, v3); // f2

			// clockwise orientation
			mesh->add_face(v0, v1, v3); // f1
			mesh->add_face(v0, v3, v2); // f2

		}
	}

	fmt::print("[{}] mesh created:\n", this->name);
	printSummary();
}

tgen::Mesh* tgen::Mesher::getMesh() {
	return mesh;
}


void tgen::Mesher::printSummary() {
	fmt::print("[{}] vertices: {}\n", this->name, mesh->number_of_vertices());
	fmt::print("[{}] edges: {}\n", this->name, mesh->number_of_edges());
	fmt::print("[{}] tfaces: {}\n", this->name, mesh->number_of_faces());
}



void tgen::Mesher::refine() {
	// CGAL::Polygon_mesh_processing::isotropic_remeshing(mesh->faces(), 1, *mesh);


	std::vector<Mesh::Face_index>  new_facets;
	std::vector<Mesh::Vertex_index> new_vertices;


	CGAL::Polygon_mesh_processing::refine(*mesh,
									faces(*mesh),
									std::back_inserter(new_facets),
									std::back_inserter(new_vertices),
									CGAL::Polygon_mesh_processing::parameters::density_control_factor(2.));
	printSummary();
}


template <typename PointIterator> 
tgen::Mesher::Construct::Construct(tgen::Mesh& mesh, PointIterator b, PointIterator e): mesh(mesh) {
	for(; b!=e; ++b){
		boost::graph_traits<tgen::Mesh>::vertex_descriptor v;
		v = add_vertex(mesh);
		mesh.point(v) = *b;
	}
}

tgen::Mesher::Construct& tgen::Mesher::Construct::operator=(const Facet f) {
	typedef boost::graph_traits<tgen::Mesh>::vertex_descriptor vertex_descriptor;
	typedef boost::graph_traits<tgen::Mesh>::vertices_size_type size_type;

	mesh.add_face(vertex_descriptor(static_cast<size_type>(f[0])),
	              vertex_descriptor(static_cast<size_type>(f[1])),
	              vertex_descriptor(static_cast<size_type>(f[2])));
	
	return *this;
}