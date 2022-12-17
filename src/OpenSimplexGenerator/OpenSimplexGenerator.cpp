#include "OpenSimplexGenerator.hpp"

tgen::OpenSimplexGenerator::OpenSimplexGenerator(){}

std::vector<tgen::Point> tgen::OpenSimplexGenerator::GenerateTerrain(int width, int heigth, 
	unsigned int seed, int octaves, int amplitude, double frequency) {

    OpenSimplex::Context context;
    OpenSimplex::Seed::computeContextForSeed(context, seed);

    std::vector<Point> points;
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            double value_noise;
            value_noise = static_cast<double>(OpenSimplex::Noise::noise4(context, (float) x * frequency, (float) y * frequency, octaves, 0.0) * amplitude);
            points.push_back(Point(x, y, value_noise));
        }
    }
    return points;

}