#include "Mesher.hpp"

#include <map>

#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/marching_tetrahedra_3.h>

#include <fmt/format.h>

// TODO: prendere spunti da https://www.youtube.com/watch?v=CSa5O6knuwI


tgen::Mesher::Mesher() {}

void tgen::Mesher::triangulate(std::vector<Point> points) {
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
void tgen::Mesher::triangulate(FT** map, const int width, const int height) {
	mesh = new Mesh();
	std::map<Point, Mesh::vertex_index> pnt2idx;
	Point** points = new Point* [width];
	for(int i = 0; i < width; i++)
		points[i] = new Point[height];

	// aggiungo tutti i punti alla mesh come vertici
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			Point p = Point(x, y, map[x][y]);
			points[x][y] = p;

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
	logger.log(log::Level::INFO, "Mesh created.");
	printSummary();
}

tgen::Mesh* tgen::Mesher::getMesh() {
	return mesh;
}


void tgen::Mesher::printSummary() {
	logger.log(log::Level::INFO, 
		fmt::format("Mesh summary: Vertices: {}; Edges: {}; Faces: {}.",
		mesh->number_of_vertices(), mesh->number_of_edges(), mesh->number_of_faces()));
}



void tgen::Mesher::refine() {
	CGAL::Polygon_mesh_processing::isotropic_remeshing(mesh->faces(), 1, *mesh);

	std::vector<Mesh::Face_index>  new_facets;
	std::vector<Mesh::Vertex_index> new_vertices;


	CGAL::Polygon_mesh_processing::refine(*mesh,
									faces(*mesh),
									std::back_inserter(new_facets),
									std::back_inserter(new_vertices),
									CGAL::Polygon_mesh_processing::parameters::density_control_factor(2.));

	logger.log(log::Level::INFO, "Mesh refined.");
	printSummary();
}

void tgen::Mesher::coloring() {
	Mesh::Property_map<Mesh::Vertex_index, CGAL::Color> color;
	bool created;
	boost::tie(color, created) = mesh->add_property_map<Mesh::Vertex_index, CGAL::Color>("v:color");

	for (auto v: mesh->vertices()) {
		auto n = CGAL::Polygon_mesh_processing::compute_vertex_normal(v, *mesh);
		Point p = mesh->point(v);
		FT tz = n.z();

		FT iz = interp(0, 360, tz);

		CGAL::Color c = CGAL::Color().set_hsv(iz, 100, 50, 255);

		auto result = c.to_rgb();
		c.set_rgb(result[0], result[1], result[2]);

		color[v] = c;
	}
	logger.log(log::Level::INFO, "Mesh colored.");
}

tgen::Point tgen::Mesher::calculateVertexposition(Point a, Point b, FT isoLevel, FT*** map) {
	// Get values at each point
	FT va = getValue(a, map);
	FT vb = getValue(b, map);
	
	// Interpolate between the two corner points based on their values to
	// estimate the point along the edge at which the value would be zero
	FT t = (isoLevel - va) / (vb - va);
	return lerp(a, b, t);
}


void tgen::Mesher::triangulate(FT ***points, const int width, 
	const int height, const int deepth, const FT isoLevel) {

	std::map<pointWrapper, Mesh::vertex_index> pnt2idx;
	Point* vertlist = new Point[12]; 
	for(int z0 = 0, z1 = 1; z1 < deepth; z0++, z1++)
		for(int y0 = 0, y1 = 1; y1 < height; y0++, y1++)
			for(int x0 = 0, x1 = 1; x1 < width; x0++, x1++) {

				Point cube[8];
				cube[0] = Point(x0, y0, z0);
				cube[1] = Point(x1, y0, z0);
				cube[2] = Point(x1, y1, z0);
				cube[3] = Point(x0, y1, z0);
				cube[4] = Point(x0, y0, z1);
				cube[5] = Point(x1, y0, z1);
				cube[6] = Point(x1, y1, z1);
				cube[7] = Point(x0, y1, z1);

				// Calculate unique index for each cube configuration.
				// There are 256 possible values (cube has 8 corners, so 2^8 possibilites).
				// A value of 0 means cube is entirely inside the surface; 255 entirely outside.
				// The value is used to look up the edge table, which indicates which edges of the cube the surface passes through.
				int cubeindex = 0;
				for (int i = 0; i < 8; i ++) {
					// Think of the configuration as an 8-bit binary number (each bit represents the state of a corner point).
					// The state of each corner point is either 0: above the surface, or 1: below the surface.
					// The code below sets the corresponding bit to 1, if the point is below the surface.
					if (getValue(cube[i], points) < isoLevel) {
						cubeindex |= (1 << i);
					}
				}

				// Get array of the edges of the cube that the surface passes through.
				const int* edgeIndices = triangulation[cubeindex];

				// Create triangles for the current cube configuration
				for (int i = 0; i < 16; i += 3) {
					
					// If edge index is -1, then no further vertices exist in this configuration
					if (edgeIndices[i] == -1) 
						break;

					// Get indices of the two corner points defining the edge that the surface passes through.
					// (Do this for each of the three edges we're currently looking at).
					int edgeIndexA = edgeIndices[i];
					int a0 = cornerIndexAFromEdge[edgeIndexA];
					int a1 = cornerIndexBFromEdge[edgeIndexA];

					int edgeIndexB = edgeIndices[i+1];
					int b0 = cornerIndexAFromEdge[edgeIndexB];
					int b1 = cornerIndexBFromEdge[edgeIndexB];

					int edgeIndexC = edgeIndices[i+2];
					int c0 = cornerIndexAFromEdge[edgeIndexC];
					int c1 = cornerIndexBFromEdge[edgeIndexC];

					// Calculate positions of each vertex.
					pointWrapper pws[3] = {	
						pointWrapper(calculateVertexposition(cube[a0], cube[a1], isoLevel, points)),
						pointWrapper(calculateVertexposition(cube[b0], cube[b1], isoLevel, points)),
						pointWrapper(calculateVertexposition(cube[c0], cube[c1], isoLevel, points))
					};

					Mesh::vertex_index vert[3];
					for(int i = 0; i < 3; i++) {
						pointWrapper pw = pws[i];
						if(pnt2idx.count(pw) <= 0) {
							vert[i] = mesh->add_vertex(pw.p);
							// CGAL::Polygon_mesh_processing::compute_vertex_normal(vert[i], *mesh);
							pnt2idx.insert({pw, vert[i]});
						} else {
							vert[i] = pnt2idx[pw];
						}
					}


					// Create triangle
					auto f = mesh->add_face(vert[2], vert[1], vert[0]);

					// CGAL::Polygon_mesh_processing::compute_face_normal(f, *mesh);

				}
			}
	printSummary();
}


#include "../../include/FastNoise/FastNoiseLite.h"
void tgen::Mesher::prova() {

	mesh = new Mesh;
	const int width = 100;
	const int height = 100;
	const int deepth = 60;

	// genero il rumore
	FastNoiseLite fbm;
	fbm.SetSeed(0);
	fbm.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	fbm.SetFractalOctaves(5);
	fbm.SetFrequency(.1);
	fbm.SetFractalType(FastNoiseLite::FractalType_FBm);
	int ray = 30;
	int offset = 30;
	FT*** points = new FT**[width];
	for(int x = 0; x < width; x++) {
		points[x] = new FT*[height];
		for(int y = 0; y < height; y++) {
			points[x][y] = new FT[deepth];
			for(int z = 0; z < deepth; z++) {
				FT eval = -fbm.GetNoise(1.0 * x, 1.0 * y, 1.0 * z) * 20 + (z - 30);
				// FT eval = std::sqrt((x-offset)*(x-offset) + (y-offset)*(y-offset) + (z-offset)*(z-offset)) - ray;
				points[x][y][z] = eval;
			}
		}
	}

	triangulate(points, width, height, deepth, 0);
	for(auto f : mesh->faces())
		CGAL::Polygon_mesh_processing::compute_face_normal(f, *mesh);
}