#include <fmt/format.h>
// #include <FastNoise/FastNoise.h>
// #include <vector>

int main(int argc, char const *argv[])
{
	auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
	auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

	fnFractal->SetSource( fnSimplex );
	fnFractal->SetOctaveCount( 5 );

	std::vector<float> noiseOutput(50 * 50);

	fnFractal->GenUniformGrid2D(noiseOutput.data(),  0, 0, 50, 50, 0.2f, 1337);
	int index = 0;
	float map[50][50];

	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			map[i][j] = noiseOutput[index++];	
			fmt::print("{}", map[i][j]);
		}
	}
	fmt::print("Ciao mondo!\n");
	return 0;
}