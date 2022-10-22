#include "KMeans.h"

int K;
DataSet xx;
Clusters clusters;

int main(int argc, char* argv[])
{
	if (argc != 6) {
		std::cerr << "usage: " << argv[0] << " K columns exclude input_path output_path" << std::endl;
		return -1;
	}

	const int K = atoi(argv[1]);
	const int columns = atoi(argv[2]);
	const int exclude = atoi(argv[3]);
	Vector::Initialize(columns, exclude);

	clusters.clear();
	xx.clear();

	const char* input_path = argv[4];
	std::ifstream input(input_path, std::ios::binary);

	const char* output_path = argv[5];
	std::ofstream output(output_path, std::ios::binary);

	std::cout << "loading from " << input_path << std::endl;
	xx.Read(input);
	input.close();

	clusters.clear();

	bool verbose = true;
	KMeans kmeans(K, xx, clusters, verbose);
	kmeans.CalculateClusters();

	std::cout << "saving to " << output_path << std::endl;
	clusters.Write(output);
	output.close();

	return 0;
}
