#ifndef MESHER_H
#define MESHER_H

#include "../terrain_generation.hpp"

#include <array>

#include <fmt/format.h>


/// Mesher
class tgen::Mesher {
	/// Classe di support al mesher per la costruzione della mesh triangolare 
	/// Classe di support al mesher per la costruzione della mesh triangolare 
    struct Construct{
        typedef std::array<std::size_t, 3> Facet;

        tgen::Mesh& mesh;

        Construct(tgen::Mesh& mesh): mesh(mesh) {}

        template <typename PointIterator> 
        Construct(tgen::Mesh& mesh, PointIterator b, PointIterator e): mesh(mesh) {
            for(; b!=e; ++b){
                boost::graph_traits<tgen::Mesh>::vertex_descriptor v;
                v = add_vertex(mesh);
                mesh.point(v) = *b;
            }
        }

        void addPoint(Point_3 point) {
            boost::graph_traits<tgen::Mesh>::vertex_descriptor v;
            v = add_vertex(mesh);
            mesh.point(v) = point;
        }

        Construct& operator*() { return *this; }
        
        Construct& operator++() { return *this; }
        
        Construct operator++(int) { return *this; }

        Construct& operator=(const Facet f) {
            typedef boost::graph_traits<tgen::Mesh>::vertex_descriptor vertex_descriptor;
            typedef boost::graph_traits<tgen::Mesh>::vertices_size_type size_type;

            mesh.add_face(vertex_descriptor(static_cast<size_type>(f[0])),
                          vertex_descriptor(static_cast<size_type>(f[1])),
                          vertex_descriptor(static_cast<size_type>(f[2])));
            
            return *this;
        }
    };

	std::string name = "Mesher";
	TGENLogger logger = TGENLogger(name);
	
	Mesh* mesh;
	struct pointWrapper {
		Point_3 p;
		pointWrapper(Point_3 p) : p(p) {}

		bool operator>(const pointWrapper & other) const {
			return std::tie(p.x(), p.y(), p.z()) > std::tie(other.p.x(), other.p.y(), other.p.z());
		}
		bool operator<(const pointWrapper & other) const {
			return std::tie(p.x(), p.y(), p.z()) < std::tie(other.p.x(), other.p.y(), other.p.z());
		}

		bool operator==(const pointWrapper & other) const {
			return std::tie(p.x(), p.y(), p.z()) == std::tie(other.p.x(), other.p.y(), other.p.z());
		}
	};


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
	void triangulate(std::vector<Point_3> points);

	/// Triangola una mappa di rumore
	void triangulate(Matrix<FT> map);

	void texturing();

	void printSummary();

	Mesh* getMesh();

	/// raffina l'ultima mesh triangolare creata
	void refine();


};


#endif