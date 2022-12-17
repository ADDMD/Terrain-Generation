cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
message("${CMAKE_HOST_SYSTEM_PROCESSOR}")

project(terrain_generation)

# Ricerca delle librerie
find_package(fmt REQUIRED)

find_package(CGAL REQUIRED)

# Percorsi delle varie cartelle
set(SOURCE_DIR "${terrain_generation_SOURCE_DIR}/src")
set(TEST_DIR "${terrain_generation_SOURCE_DIR}/test")

# Aggiunge le cartelle del progetto
include_directories(
  "${SOURCE_DIR}"
  "${TEST_DIR}"
)

# Set delle performance per CGAL
set(CMAKE_BUILD_TYPE Build) 

# Specifiche del C++ 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -pedantic -Werror -std=c++17 -O3 -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${terrain_generation_SOURCE_DIR}/bin")

# Selezione dei file da compilare
file(GLOB SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*/*.hpp" "${SOURCE_DIR}/*/*.cpp")
file(GLOB TESTS CONFIGURE_DEPENDS "${TEST_DIR}/*.cpp")


# Creazione degli eseguibili
add_executable(terrain_generation "${SOURCES}")
add_executable(test_terrain_generation "${TESTS}")

# Aggiungiamo header only
# add_library(stb_perlin INTERFACE)
# add_library(PerlinNoise INTERFACE)
add_library(fbm INTERFACE)

# Including delle directories
# target_include_directories(stb_perlin INTERFACE "${terrain_generation_SOURCE_DIR}/include/stb_perlin")
# target_include_directories(PerlinNoise INTERFACE "${terrain_generation_SOURCE_DIR}/include/PerlinNoise")
target_include_directories(fbm INTERFACE "${terrain_generation_SOURCE_DIR}/include/fbm")

# Linking delle librerie
target_link_libraries(terrain_generation 
  fmt::fmt-header-only
  CGAL::CGAL
  fbm
)
target_link_libraries(test_terrain_generation fmt::fmt)