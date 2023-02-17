#include "Mesher.hpp"

#include <map>

#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>

#include <fmt/format.h>

// TODO: prendere spunti da https://www.youtube.com/watch?v=CSa5O6knuwI


tgen::Mesher::Mesher() {}

void tgen::Mesher::triangulate(std::vector<Point_3> points) {
	mesh = new Mesh();

	Construct construct(*mesh, points.begin(), points.end());
	CGAL::advancing_front_surface_reconstruction(points.begin(), points.end(), construct);

	printSummary();
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
void tgen::Mesher::triangulate(Matrix<FT> map) {
	unsigned int width = map.size();
	unsigned int height = map[0].size();

	mesh = new Mesh();
	
	std::map<Point_3, Mesh::vertex_index> pnt2idx;
	auto points = generateMatrix<Point_3>(width, height);

	// aggiungo tutti i punti alla mesh come vertici
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			Point_3 p = Point_3(x, y, map[x][y]);
			points[x][y] = p;

			pnt2idx.insert({p, mesh->add_vertex(p)});
		}
	}

	// triangolo le facce
	for(int x0 = 0, x1 = 1; x1 < width; x0++, x1++) {
		for(int y0 = 0, y1 = 1; y1 < height; y0++, y1++) {
			
			Point_3 p0 = points[x0][y0];
			Point_3 p1 = points[x1][y0];
			Point_3 p2 = points[x0][y1];
			Point_3 p3 = points[x1][y1];

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
	logger.log(logtg::Level::INFO, "Mesh created.");
	printSummary();
}


tgen::Mesh* tgen::Mesher::getMesh() {
	return mesh;
}


void tgen::Mesher::printSummary() {
	logger.log(logtg::Level::INFO, 
		fmt::format("Mesh summary: Vertices: {}; Edges: {}; Faces: {}.",
		mesh->number_of_vertices(), mesh->number_of_edges(), mesh->number_of_faces()));
}



void tgen::Mesher::refine() {
	CGAL::Polygon_mesh_processing::split_long_edges(mesh->edges(), 1.5, *mesh);
 	CGAL::Polygon_mesh_processing::isotropic_remeshing(mesh->faces(), 1, *mesh);

	// std::vector<Mesh::Face_index>  new_facets;
	// std::vector<Mesh::Vertex_index> new_vertices;


	// CGAL::Polygon_mesh_processing::refine(*mesh,
	// 								faces(*mesh),
	// 								std::back_inserter(new_facets),
	// 								std::back_inserter(new_vertices),
	// 								CGAL::Polygon_mesh_processing::parameters::density_control_factor(2.));
	CGAL::Polygon_mesh_processing::smooth_shape(mesh->faces(), *mesh, 0.1);
	logger.log(logtg::Level::INFO, "Mesh refined.");
	printSummary();
}