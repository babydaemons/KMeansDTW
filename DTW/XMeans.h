#pragma once
#include "KMeans.h"

class XMeans {
public:
	XMeans(DataSet& dataSet, Clusters& clusters, int level, int divide_label, int& start_label, std::string verbose, int min_cluster_size = 0)
	 : dataSet(dataSet), clusters(clusters), level(level), divide_label(divide_label), start_label(start_label), verbose(verbose), min_cluster_size(min_cluster_size == 0 ? static_cast<int>(dataSet.size() >> level) : min_cluster_size)
	{
		KMeans kmeans(N, divide_label, start_label, dataSet, clusters, verbose.c_str());
		kmeans.CalculateClusters();
	}

	const Clusters& CalculateClusters() {
		DivideClusters();

		for (auto cluster = clusters.begin(); cluster != clusters.end(); ++cluster) {
			cluster->CountClear();
		}

		for (auto x = dataSet.begin(); x != dataSet.end(); ++x) {
			auto label = x->label;
			clusters[label].CountUp();
		}

		for (auto cluster = clusters.begin(); cluster != clusters.end();) {
			if (cluster->Count() == 0) {
				cluster = clusters.erase(cluster);
			}
			else {
				++cluster;
			}
		}

		std::cout << "# XMeans result:" << std::endl;
		for (auto cluster = clusters.begin(); cluster != clusters.end(); ++cluster) {
			const auto x = cluster->Mean();
			auto profit = x[Vector::COLUMNS - 1][0];
			std::cout << cluster->label << '\t' << cluster->Count() << '\t' << profit << std::endl;
		}

		return clusters;
	}

	static const int N = 2;

private:
	void DivideClusters() {
		if (level > 0) {
			verbose += '#';

			auto divide_label0 = start_label + 0;
			auto divide_label1 = start_label + 1;

			if (clusters[divide_label0].Count() > min_cluster_size) {
				start_label += N;
				XMeans xmeans0(dataSet, clusters, level - 1, divide_label0, start_label, verbose.c_str(), min_cluster_size);
				xmeans0.DivideClusters();
			}

			if (clusters[divide_label1].Count() > min_cluster_size) {
				start_label += N;
				XMeans xmeans1(dataSet, clusters, level - 1, divide_label1, start_label, verbose.c_str(), min_cluster_size);
				xmeans1.DivideClusters();
			}
		}
	}

private:
	DataSet& dataSet;
	Clusters& clusters;
	const int level;
	const int divide_label;
	int& start_label;
	std::string verbose;
	int min_cluster_size;
};