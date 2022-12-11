#include "mesher.hpp"

#include <CGAL/Advancing_front_surface_reconstruction.h>

tgen::Mesher::Mesher(){}

tgen::Mesh tgen::Mesher::triangulate(std::vector<Point> points) {
	Mesh m;
	Construct construct(m, points.begin(), points.end());
	CGAL::advancing_front_surface_reconstruction(points.begin(), points.end(), construct);

	return m;
}


template <typename PointIterator> 
tgen::Mesher::Construct::Construct(tgen::Mesh& mesh, PointIterator b, PointIterator e): mesh(mesh) {
	for(; b!=e; ++b){
		boost::graph_traits<tgen::Mesh>::vertex_descriptor v;
		v = add_vertex(mesh);
		mesh.point(v) = *b;
	}
}

tgen::Mesher::Construct& tgen::Mesher::Construct::operator=(const Facet f){
	typedef boost::graph_traits<tgen::Mesh>::vertex_descriptor vertex_descriptor;
	typedef boost::graph_traits<tgen::Mesh>::vertices_size_type size_type;

	mesh.add_face(vertex_descriptor(static_cast<size_type>(f[0])),
	              vertex_descriptor(static_cast<size_type>(f[1])),
	              vertex_descriptor(static_cast<size_type>(f[2])));
	
	return *this;
}