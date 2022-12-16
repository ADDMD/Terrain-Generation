#ifndef TERRAIN_GENERATION_H
#define TERRAIN_GENERATION_H

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

namespace tgen {
	/// kernel
	typedef CGAL::Cartesian<double> Kernel;

	/// kernel per calcoli più precisi
	// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

	/// punto 3D
	typedef Kernel::Point_3 Point;

	/// mesh triangolare
	typedef CGAL::Surface_mesh<Point> Mesh;
	
	class Mesher;
	class PerlinGenerator;
	class fbmGenerator;

}


#endif