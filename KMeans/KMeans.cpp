#include "KMeans.h"

int K;
DataSet xx;
Clusters clusters;

extern "C" {
	void _stdcall KMeansInitialize(int k, int columns, int exclude);
	void _stdcall KMeansClear();
	void _stdcall KMeansAppend(const double a[]);
	int  _stdcall KMeansClustering();
	void _stdcall KMeansFetch(int index, double a[]);
}

void _stdcall KMeansInitialize(int k, int columns, int exclude)
{
	K = k;
	Vector::Initialize(columns, exclude);
}

void _stdcall KMeansClear()
{
	clusters.clear();
	xx.clear();
}

void _stdcall KMeansAppend(const double a[])
{
	Vector x;
	for (size_t i = 0; i < x.size(); ++i) {
		x[i] = a[i];
	}

	xx.push_back(x);
}

int _stdcall KMeansClustering()
{
	clusters.clear();

#if defined(_DEBUG)
	bool verbose = false;
#else
	bool verbose = false;
#endif
	KMeans kmeans(K, xx, clusters, verbose);
	kmeans.CalculateClusters();

	return static_cast<int>(clusters.size());
}

void _stdcall KMeansFetch(int index, double a[])
{
	const Vector& x = clusters[index];
	for (size_t i = 0; i < x.size(); ++i) {
		a[i] = x[i];
	}
}

#if defined(_DEBUG) && !defined(KMeans_EXE)

int main(int argc, char* argv[])
{
	const int K = 32;
	const int columns = 803;
	const int exclude = 11;
	KMeansInitialize(K, columns, exclude);
	KMeansClear();

	const wchar_t* input_path = L"USDJPY-IN.tsv";
	std::ifstream input(input_path);
	Vector x;
	while (!input.eof()) {
		input >> x;
		KMeansAppend(reinterpret_cast<const double*>(&(x[0])));
	}
	input.close();

	KMeansClustering();
	const wchar_t* output_path = L"USDJPY-OUT.tsv";

	std::ofstream output(output_path);
	output << clusters;
	output.close();

	return 0;
}

#endif

#if defined(KMeans_EXE)

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

#endif
