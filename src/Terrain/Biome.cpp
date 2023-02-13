#include "./Biome.hpp"

tgen::Biome::Biome(std::set<Point_2>& points, BiomeType type) {
	this->points = points;
	this->type = type;
	this->centroid = computeCentroid();

}

bool tgen::Biome::contains(Point_2 point) {
	return points.find(point) != points.end();
}

bool tgen::Biome::addPoint(Point_2 point) {
	bool result = points.insert(point).second;
	
	if (result)
		this->centroid = computeCentroid();
	
	return result;
}

void tgen::Biome::addPoints(std::set<Point_2> points) {
	this->points.insert(points.begin(), points.end());

	this->centroid =computeCentroid();
}


tgen::Point_2 tgen::Biome::computeCentroid() {
	FT dx = 0;
	FT dy = 0;

	for(auto point : points) {
		dx += point.x();
		dy += point.y();
	}
	int size = points.size();

	return Point_2(dx/size, dy/size);
}