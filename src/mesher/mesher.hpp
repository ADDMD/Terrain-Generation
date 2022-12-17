#ifndef MESHER_H
#define MESHER_H

#include "../terrain_generation.hpp"

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

public:
	Mesher();

	/**
	 * Triangola una nuvola di punti e restituisce una mesh triangolare
	 * 
	 * tre alternative possibili per "points -> mesh" con CGAL
	 * - Scale-Space Surface Reconstruction
	 * - Advancing Front Surface Reconstruction (Utilizzata)
	 * - Poisson Surface Reconstruction
	 * fonte: https://stackoverflow.com/questions/34887019/surface-mesh-from-point-cloud
	 */
	Mesh triangulate(std::vector<Point> points);

	Mesh triangulate(Point** points, int width, int height);

	/// raffina una mesh triangolare
	void refine(Mesh& mesh);


};


#endif