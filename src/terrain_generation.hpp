#ifndef TERRAIN_GENERATION_H
#define TERRAIN_GENERATION_H

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include "./Utility/Logger.hpp"
#include "./Utility/Config.hpp"

namespace tgen {
	/// kernel
	typedef CGAL::Cartesian<double> Kernel;

	/// kernel per calcoli più precisi
	// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

	/// punto 3D
	typedef Kernel::Point_3 Point;

	/// VETTORE 3D
	typedef Kernel::Vector_3 Vector;

	/// mesh triangolare
	typedef CGAL::Surface_mesh<Point> Mesh;

	typedef Kernel::FT FT;
	
	class Mesher;
	class NoiseGenerator;
	
	class Noise;
	class PerlinNoise;
	class SimplexNoise;
	class CellularNoise;
	class OpenSimplexNoise;
	class FBMNoise;
	class Terrain;
	class TerrainGenerator;


	class TGENLogger : public logtg::Logger {

	public:
		TGENLogger(std::string className) 
			: logtg::Logger(className, std::vector<std::ostream*>{
					&std::cout, 
					new std::ofstream(Config("../config.yaml")["logging.file"], std::ios::app)
				}) {}
	};

}


#endif
