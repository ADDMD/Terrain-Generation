#include "./Biome.hpp"

tgen::Biome::Biome(std::vector<Point_3> points) {
	this->points = std::set(points.begin(), points.end());
	this->centroid = computeCentroid();

}

bool tgen::Biome::contains(Point_3 point) {
	return points.find(point) != points.end();
}

bool tgen::Biome::addPoint(Point_3 point) {
	bool result = points.insert(point).second;
	
	if (result)
		this->centroid = computeCentroid();
	
	return result;
}


tgen::Point_3 tgen::Biome::computeCentroid() {
	FT dx = 0;
	FT dy = 0;
	FT dz = 0;

	for(auto point : points) {
		dx += point.x();
		dy += point.y();
		dz += point.z();
	}
	int size = points.size();

	return Point_3(dx/size, dy/size, dz/size);
}