#include "XMeans.h"

int main(int argc, char* argv[])
{
	if (argc != 6) {
		std::cerr << "usage: " << argv[0] << " level columns exclude input_path output_path" << std::endl;
		return -1;
	}

	const int level = atoi(argv[1]);
	const int columns = atoi(argv[2]);
	const int exclude = atoi(argv[3]);
	Vector::Initialize(columns, exclude);

	const char* input_path = argv[4];
	std::ifstream input(input_path, std::ios::binary);

	const char* output_path = argv[5];
	std::ofstream output(output_path, std::ios::binary);

	std::cout << "loading from " << input_path << std::endl;
	DataSet xx;
	xx.Read(input);
	input.close();

	Clusters clusters;
	auto divide_label = -1;
	auto start_label = 0;
	XMeans xmeans(xx, clusters, level, divide_label, start_label, "#");
	xmeans.CalculateClusters();

	std::cout << "saving to " << output_path << std::endl;
	clusters.Write(output);
	output.close();

	return 0;
}
