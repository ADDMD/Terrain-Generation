#include "../terrain_generation.hpp"

class tgen::fbmGenerator{
	int dim;
	int nx;
	double H;
	int width;
	int lenght;
public:
	// La dimensione deve essere necessariamente 3
	//! 
	//!
	fbmGenerator();
	std::vector<double> generate_terrain();

};