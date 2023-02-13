#ifndef BIOME_H
#define BIOME_H

#include "../terrain_generation.hpp"

class tgen::Biome {
public:	
	enum BiomeType {
		Mountains = 0,
		Hills,
		Plains,
		Snowy
	};

private:
	Point_2 centroid;

	std::set<Point_2> points;

	Point_2 computeCentroid();
	BiomeType type;


public:
	Biome() {}
	Biome(std::set<Point_2>& points, BiomeType type);

	Point_2 getCentroid() {
		return centroid;
	}
	std::set<Point_2> getPoints() {
		return points;
	}

	BiomeType getType() {
		return type;
	}

	bool addPoint(Point_2 point);
	void addPoints(std::set<Point_2> points);

	bool contains(Point_2 point);


	
};


#endif