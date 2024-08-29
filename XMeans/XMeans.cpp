#include "XMeans.h"

const bool debugging = false;

int main(int argc, char* argv[])
{
	if (argc != 7) {
		std::cerr << "usage: " << argv[0] << " wait level columns exclude input_path output_path" << std::endl;
		return -1;
	}

	const int wait = atoi(argv[1]);
	if (debugging) std::cout << "wait = " << wait << std::endl;

	const int level = atoi(argv[2]);
	if (debugging) std::cout << "level = " << level << std::endl;

	const int columns = atoi(argv[3]);
	if (debugging) std::cout << "columns = " << columns << std::endl;

	const int exclude = atoi(argv[4]);
	if (debugging) std::cout << "exclude = " << exclude << std::endl;

	Vector::Initialize(columns, exclude);

	const char* input_path = argv[5];
	if (debugging) std::cout << "input_path = " << input_path << std::endl;
	std::ifstream input(input_path, std::ios::binary);

	const char* output_path = argv[6];
	if (debugging) std::cout << "output_path = " << output_path << std::endl;
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

	if (wait != 0) {
		std::cout << "type enter key to exit..." << std::flush;
		std::string input;
		std::cin >> input;
	}

	return 0;
}
