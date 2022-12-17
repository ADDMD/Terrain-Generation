#include "fractionalBrownianMotion.hpp"
#include "../config.hpp"
#include "../terrain_generation.hpp"
#include <fmt/format.h>
#include "fbm/generate_normal_random.hpp"
#include "fbm/fbm.hpp"
#include <exception>

#define FBM_ASSERT_CONFIGFILE_OPEN(x) {\
	if(not x.is_open()) {\
		std::stringstream message; \
		message << "Config file must be open to get parameters.\n\n" \
		<< "In" << __FILE__ << "at" << __LINE__ << "\n"; \
		throw std::runtime_error(message.str().c_str());\
	}\
}

#define FBM_ASSERT_DIMENSION_3(x) {\
	if(x != 3) {\
		std::stringstream message;\
		message << "Dimension must be 3\n" \
		<< "Given: " << x << "\n\n" \
		<< "In " << __FILE__ << "at " << __LINE__ << "\n"; \
		throw std::runtime_error(message.str().c_str()); \
	}\
}

tgen::fbmGenerator::fbmGenerator(){
	config c("../config.txt");
	FBM_ASSERT_CONFIGFILE_OPEN(c);
	dim = std::stoi(c["dimension"]);
	FBM_ASSERT_DIMENSION_3(dim);
	nx = std::stoi(c["nx"]);
	H = std::stof(c["H"]);
	lenght = std::stoi(c["lenght"]);
	width = std::stoi(c["width"]);;
}

// TODO: studia parametri e come funziona questa libreria
std::vector<double> tgen::fbmGenerator::generate_terrain(){
	std::vector<double> points = {};

	auto X = fbm::generate_normal_random((nx+1)*(nx+1)*(nx+1));
    auto fractional_bridge = fbm::fractional_brownian_bridge_3d(H, nx, X);

    // for (int i = 0; i < lenght; ++i)
    // {
    // 	for (int i = 0; i < width; ++i)
    // 	{
    		
    // 	}
    // }
    // for (const auto d : fractional_bridge) {
    // 	points.push_back(d);
    // }

	return fractional_bridge;
}