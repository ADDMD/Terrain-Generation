#ifndef TERRAIN_GENERATION_H
#define TERRAIN_GENERATION_H

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include "./Utility/Logger.hpp"
#include "./Utility/Config.hpp"

namespace tgen {
	// colori utili per la stampa della mappa
	static CGAL::Color green = CGAL::Color(115,169,102);
	static CGAL::Color darkgreen = CGAL::Color(72,111,56);
	static CGAL::Color grey = CGAL::Color(150,145,142);
	static CGAL::Color white = CGAL::Color(255,255,255);
	static CGAL::Color blue = CGAL::Color(68,68,122,255);
	static CGAL::Color giallastra = CGAL::Color(247,218,158,255);
	static CGAL::Color snow = CGAL::Color(255,250,250);
	static CGAL::Color mountain = CGAL::Color(151,124,83);
	static CGAL::Color grass = CGAL::Color(126, 200, 80);

	/// kernel
	typedef CGAL::Cartesian<double> Kernel;

	/// kernel per calcoli più precisi
	// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

	/// punto 2D
	typedef Kernel::Point_2 Point_2;

	/// punto 3D
	typedef Kernel::Point_3 Point_3;

	/// VETTORE 3D
	typedef Kernel::Vector_3 Vector;

	typedef Kernel::Vector_2 Vector_2;

	/// mesh triangolare
	typedef CGAL::Surface_mesh<Point_3> Mesh;

	typedef Kernel::FT FT;

	typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
	typedef boost::graph_traits<Mesh>::vertex_descriptor vertex_descriptor;
	typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;
	
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
	class Biome;

	// template<typename T>
	// constexpr size_t maxLength(size_t n, size_t m_size=0) {
	//     return n==0 ? m_size : maxLength<T>(n-1, m_size << 8 | std::numeric_limits<T>::max());
	// }
	// template <size_t width, size_t height, typename T>
	// using Matrix = std::array<std::array<T, height>, width>;

	template <typename T>
	using Matrix = std::vector<std::vector<T>>;

	template <typename T>
	inline Matrix<T> generateMatrix(int width, int height) {
		Matrix<T> result(width);
		std::fill(result.begin(), result.end(), std::vector<T>(height));
		return result;
	}

	template <typename T>
	inline T lerp(T start, T end, tgen::FT t) {
		return start + t * (end - start);
	}

	template <typename T>
	inline T interp(T start, T end, tgen::FT t) {
		tgen::FT ti = ((6 * t - 15) * t + 10) * t * t * t;
		return lerp(start, end, ti);
	}

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
