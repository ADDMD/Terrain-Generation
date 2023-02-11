#ifndef BIOME_H
#define BIOME_H

#include "../terrain_generation.hpp"

class tgen::Biome {


	Point_3 centroid;

	std::set<Point_3> points;

	Point_3 computeCentroid();

public:
	Biome() {}
	Biome(std::vector<Point_3> points);

	Point_3 getCentroid() {
		return centroid;
	}
	std::set<Point_3> getPoints() {
		return points;
	}

	bool addPoint(Point_3 point);

	bool contains(Point_3 point);

	enum BiomeType {
		Mountains = 0,
		Hills,
		Plains,
		Snowy
	};
};


#endif