#ifndef MESHER_H
#define MESHER_H

#include "../terrain_generation.hpp"

#include <fmt/format.h>

#include <array>

#include <CGAL/Advancing_front_surface_reconstruction.h>

/// Mesher
class tgen::Mesher {
	

	/// Classe di support al mesher per la costruzione della mesh triangolare 
	struct Construct{
		typedef std::array<std::size_t, 3> Facet;

		tgen::Mesh& mesh;

		template <typename PointIterator>
		Construct(tgen::Mesh& mesh, PointIterator b, PointIterator e);

		Construct& operator=(const Facet f);

		Construct& operator*() { return *this; }
		
		Construct& operator++() { return *this; }
		
		Construct operator++(int) { return *this; }
	};

	std::string name = "mesher";
	
	Mesh* mesh;


public:
	Mesher();

	/**
	 * Triangola una nuvola di punti
	 * 
	 * tre alternative possibili per "points -> mesh" con CGAL
	 * - Scale-Space Surface Reconstruction
	 * - Advancing Front Surface Reconstruction (Utilizzata)
	 * - Poisson Surface Reconstruction
	 * fonte: https://stackoverflow.com/questions/34887019/surface-mesh-from-point-cloud
	 */
	void triangulate(std::vector<Point> points);

	void triangulate(Point** points, int width, int height);

	void printSummary();

	Mesh* getMesh();

	/// raffina l'ultima mesh triangolare creata
	void refine();


};


#endif