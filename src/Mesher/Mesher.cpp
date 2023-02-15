#include "Mesher.hpp"

#include <map>

#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/marching_tetrahedra_3.h>

//necessario per lettura di file .obj
//#include <CGAL/Three/reader/OBJ.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/aff_transformation_tags.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Polygon_mesh_processing/transform.h>


#include <CGAL/Surface_mesh.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Surface_mesh_parameterization/parameterize.h>

#include <fmt/format.h>

#include <cmath>
#include <random>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polyhedron_3<K> Polyhedron;

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
void tgen::Mesher::triangulate(Matrix<FT> &map, Matrix<FT> &humidity, Matrix<FT> &temperature) {
	Config conf("../config.yaml");
	std::string path_tree = conf["assets.tree"];

	unsigned int width = map.size();
	unsigned int height = map[0].size();

	mesh = new Mesh();
	
	std::map<Point_3, Mesh::vertex_index> pnt2idx;
	// Crea una Point_3 per i punti in cui verrà aggiunto un albero
  	std::list<Point_3> pnt2tree;
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

			//INIZIO PARTE RELATIVA PER LA GENERAZIONE DEGLI ALBERI
			/*
				CGAL::Vector_3 normale_1= CGAL::normal(p0,p1,p3);
				CGAL::Vector_3 normale_2= CGAL::normal(p0,p3,p2);
				fmt::print("Normale x: {}\n",normale_1.x());
				fmt::print("Normale y: {}\n",normale_1.y());
				fmt::print("Normale z: {}\n",normale_1.z());
			*/

			Mesh::vertex_index v0 = pnt2idx.find(p0)->second;
			Mesh::vertex_index v1 = pnt2idx.find(p1)->second;
			Mesh::vertex_index v2 = pnt2idx.find(p2)->second;
			Mesh::vertex_index v3 = pnt2idx.find(p3)->second;

			// counterclockwise orientation
			// mesh->add_face(v0, v3, v1); // f1
			// mesh->add_face(v0, v2, v3); // f2

			// clockwise orientation
			auto f1 = mesh->add_face(v0, v1, v3); // f1
			auto f2 = mesh->add_face(v0, v3, v2); // f2


			auto normal1 = CGAL::Polygon_mesh_processing::compute_face_normal(f1, *mesh);
			auto normal2 = CGAL::Polygon_mesh_processing::compute_face_normal(f2, *mesh);


			if (normal1.z()>=0.7 && normal1.z() <=1){
				
				//generazione di numeri casuali tra 0 e 1
				std::mt19937 generator(std::random_device{}());
    			std::uniform_real_distribution<double> distribution(0, 1);
    			//generazione numero casuale
    			auto num_ran = distribution(generator);
				
				if (temperature[x0][y0] < 0.7 && humidity[x0][y0] > 0.5){
					//creazione albero con alta probabilità grazie ad un numero
					//random che deve essere maggiore di 0.5
					if (num_ran>=0.5)
					{
						pnt2tree.push_back(p0);
					}
				}
				else{
					//creazione albero con alta probabilità grazie ad un numero
					//random che deve essere maggiore di 0.8
					if (num_ran>=0.8)
					{
						pnt2tree.push_back(p0);
					}
				}
			}

			if (normal2.z()>=0.7 && normal2.z() <=1){

				//generazione di numeri casuali tra 0 e 1
				std::mt19937 generator(std::random_device{}());
    			std::uniform_real_distribution<double> distribution(0, 1);
    			//generazione numero casuale
    			auto num_ran = distribution(generator);

				if (temperature[x0][y0] < 0.7 && humidity[x0][y0] > 0.5){
					//creazione albero con alta probabilità grazie ad un numero
					//random che deve essere maggiore di 0.5
					if (num_ran>=0.5)
					{
						pnt2tree.push_back(p0);
					}
				}
				else{
					//creazione albero con alta probabilità grazie ad un numero
					//random che deve essere maggiore di 0.8
					if (num_ran>=0.8)
					{
						pnt2tree.push_back(p0);
					}
				}
			}

			//FINE PARTE RELATIVA PER LA GENERAZIONE DEGLI ALBERI






		}
	}

	// Crea un oggetto Polyhedron vuoto
	Mesh tree_mesh;

	Mesh tree_map;

	// Leggi il file .obj
	std::ifstream input(path_tree);
	if (!input || !CGAL::IO::read_OBJ(input, tree_mesh)) {
		std::cerr << "Errore nella lettura del file obj" << std::endl;
	}

    // Itera sulla lista e stampa i suoi elementi
	for (auto it : pnt2tree) {
		//auto transf = CGAL::Translation(it);
		//CGAL::Aff_transformation_3<Kernel>
		/*CGAL::Aff_transformation_3<Kernel> translate(1, 0, 0, it.x(),
													 0, 1, 0, it.y(),
													 0, 0, 1, it.z(),
													 0, 0, 0, 1);
*/
		// Crea una trasformazione di oggetti che sposterà la mesh in un punto specifico
		CGAL::Aff_transformation_3<Kernel> transformation(CGAL::TRANSLATION, Vector(it.x(), it.y(), it.z()));

		// Applica la trasformazione alla mesh
		//mesh->transform(transformation);
		
		CGAL::Polygon_mesh_processing::transform(transformation, tree_mesh);

		// Applica la trasformazione all'albero
		//CGAL::Polygon_mesh_processing::transform(transf, tree_mesh);
		//CGAL::make_surface_mesh(mesh, tree_mesh);
		mesh->join(tree_mesh);
		std::cout << it << std::endl;
	}

	logger.log(logtg::Level::INFO, "Mesh created.");
	printSummary();
}

tgen::Mesh* tgen::Mesher::getMesh() {
	return mesh;
}

/*
tgen::Point_3 tgen::Mesher::crossProduct(Point_3 a, Point_3 b) {
    auto x = a.y() * b.z() - a.z() * b.y();
    auto y = a.z() * b.x() - a.x() * b.z();
    auto z = a.x() * b.y() - a.y() * b.x();
	
    tgen::Point_3 result(x, y, z);
    return result;
}


tgen::Point_3 tgen::Mesher::findNormal(Point_3 p1, Point_3 p2, Point_3 p3) {
    auto x = p2.x() - p1.x();
    auto y = p2.y() - p1.y();
    auto z = p2.z() - p1.z();
    tgen::Point_3 v1(x,y,z);

    x = p3.x() - p1.x();
    y = p3.y() - p1.y();
    z = p3.z() - p1.z();

    tgen::Point_3 v2(x,y,z);

    tgen::Point_3 normal = crossProduct(v1, v2);
    return normal;
}*/


void tgen::Mesher::printSummary() {
	logger.log(logtg::Level::INFO, 
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

	logger.log(logtg::Level::INFO, "Mesh refined.");
	printSummary();
}

void tgen::Mesher::coloring() {
	Mesh::Property_map<Mesh::Vertex_index, CGAL::Color> color;
	bool created;
	boost::tie(color, created) = mesh->add_property_map<Mesh::Vertex_index, CGAL::Color>("v:color");

	for (auto v: mesh->vertices()) {
		auto n = CGAL::Polygon_mesh_processing::compute_vertex_normal(v, *mesh);
		Point_3 p = mesh->point(v);
		FT tz = n.z();

		// FT iz = interp(0, 360, tz);
		CGAL::Color c;
		if(tz <= 0.5) c = grey;
		else if (tz > 0.5 & tz <= 0.85){
				if ((p.z() / 55) < 0.05) c = giallastra;
		 		else c = darkgreen;
			}
		else if (tz > 0.85 & tz <= 1){
		 		if ((p.z() / 55) < 0.05) c = giallastra;
		 		else c = green;
		 	}

		auto result = c.to_rgb();
		c.set_rgb(result[0], result[1], result[2]);

		color[v] = c;

	}
	logger.log(logtg::Level::INFO, "Mesh colored.");
}

// void tgen::Mesher::texturing(){
// 	halfedge_descriptor bhd = CGAL::Polygon_mesh_processing::longest_border(*mesh).first;

// 	typedef Mesh::Property_map<vertex_descriptor, Point_2> UV_pmap;
// 	UV_pmap uvmap = mesh->add_property_map<vertex_descriptor, Point_2>("v:uv").first;

// 	CGAL::Surface_mesh_parameterization::parameterize(*mesh, bhd, uvmap);
// }
