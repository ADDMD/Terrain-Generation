//#include <fmt/format.h>
#include "../../../../../librerie/fmt-9.0.0/include/fmt/format.h"
#include "../../../../../librerie/cgal-5.5.1/include/CGAL/Point_3.h"
#include <vector>
#include <iostream>
#define DB_PERLIN_IMPL
#include "../include/db_perlin.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "../include/stb_perlin.h"


int main() {   
    // Rumore
    std::vector<double> noise;
    
    // Dimensioni terreno
    int heigth = 10;
    int width = 10;

    // Generazione rumore
    for (int i = 0; i < heigth; i++) {
        for (int j = 0; j < width; j++) {
            noise.push_back(stb_perlin_noise3(i,j,0.9f,256,256,256));
        }
    }

    for (int i = 0; i < noise.size(); i++) {
        fmt::print("{:f}\n", noise.at(i));
    }
    fmt::print("\n");
    return 0;
}