#ifndef MESHER_H
#define MESHER_H

#include "../terrain_generation.hpp"

#include <array>
#include <CGAL/Advancing_front_surface_reconstruction.h>

namespace tgen {
	
	class Mesher {
		
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

	public:
		Mesher();

		Mesh triangulate(std::vector<Point> points);

		Mesh refine(Mesh mesh);


	};


};


#endif