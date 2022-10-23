#pragma once
#include <vector>
#include <limits>
#include <random>
#include <thread>
#include <cassert>
#include "DataSet.h"
#include "Cluster.h"

class KMeans {
public:
	KMeans(int k, int divide_label, int start_label, DataSet& dataSet, Clusters& clusters, const char* verbose = nullptr)
		: K(k), divide_label(divide_label), start_label(start_label), ROWS(0), x(dataSet), clusters(clusters), iteration(0), max_iteration(0), cluster_changed(0), max_cluster_changed(0), verbose(verbose)
	{
		for (size_t i = 0; i < x.size(); ++i) {
			if (x[i].label == divide_label) {
				++ROWS;
			}
		}
		max_cluster_changed = ROWS / 64;
		max_iteration = static_cast<int>(std::ceil(5.0 * std::log2(ROWS)));

		std::mt19937 mt(0x12345678);
		std::uniform_int_distribution<int> dist(0, 256 * K - 1);
		auto rows = 0;
		for (size_t i = 0; i < x.size(); ++i) {
			if (x[i].label == divide_label) {
				auto label = dist(mt) / 256;
				if (label >= K) {
					label = K - 1;
				}
				x[i].label = start_label + label;
				assert(start_label <= x[i].label && x[i].label < start_label + K);
				++rows;
			}
		}
		assert(rows == ROWS);

		clusters.resize(start_label + K);
		for (auto i = start_label; i < start_label + K; ++i) {
			clusters[i].label = i;
			clusters[i] = 0;
		}

		UpdateClusterCenter();
	}

	const Clusters& CalculateClusters() {
		while (true) {
			++iteration;

			cluster_changed = 0;
			auto cores = static_cast<int>(std::thread::hardware_concurrency());
			std::vector<int> part_cluster_changed(cores);
			std::vector<std::thread*> threads(cores);
			for (auto i = 0; i < cores; ++i) {
				threads[i] = new std::thread(&ThreadEntry, this, i, cores, std::ref(part_cluster_changed[i]));
			}
			for (auto i = 0; i < cores; ++i) {
				threads[i]->join();
				delete threads[i];
				cluster_changed += part_cluster_changed[i];
			}
#if defined(VERBOSE)
			std::cout << "done!" << std::endl;
#endif

			UpdateClusterCenter();

			if (cluster_changed <= max_cluster_changed) {
				break;
			}

			if (iteration > max_iteration) {
				break;
			}
		}

#if 0
		for (size_t i = 0; i < clusters.size(); ++i) {
			if (clusters[i].Count() == 0) {
				clusters.erase(clusters.begin() + i);
			}
		}
#endif

		return clusters;
	}

private:
	static void ThreadEntry(KMeans* self, int thread, int cores, int& part_cluster_changed) {
		self->AssignNearestCluster(thread, cores, part_cluster_changed);
	}

	void AssignNearestCluster(int thread, int cores, int& part_cluster_changed) {
		auto partition = ROWS / cores;
		auto start = thread * partition;
		auto end = (thread + 1) * partition;
		if (thread == cores - 1) {
			end = ROWS;
		}

		part_cluster_changed = 0;
		for (auto i = start; i < end; ++i) {
			if (x[i].label < start_label || start_label + K <= x[i].label) {
				continue;
			}

			auto min_dist = std::numeric_limits<double>::max();
			auto new_label_index = std::numeric_limits<int>::max();
			for (auto j = start_label; j < start_label + K; ++j) {
				auto dist = Dist(clusters[j], x[i]);
				if (dist < min_dist) {
					min_dist = dist;
					new_label_index = j;
				}
			}
			auto new_label = clusters[new_label_index].label;
			if (x[i].label != new_label) {
				++part_cluster_changed;
			}
			x[i].label = new_label;
#if defined(VERBOSE)
			std::cout << '.' << std::flush;
#endif
		}
	}

	void UpdateClusterCenter() {
		#pragma omp parallel for
		for (int i = start_label; i < start_label + K; ++i) {
			clusters[i].Clear();
		}

		for (size_t i = 0; i < x.size(); ++i) {
			auto label = x[i].label;
			if (label < start_label || start_label + K <= label) {
				continue;
			}
			clusters[label] += x[i];
		}

		auto rows = 0;
		for (int i = start_label; i < start_label + K; ++i) {
			clusters[i].Mean();
			rows += clusters[i].Count();
		}

		if (verbose != nullptr) {
			std::cout << verbose << " dataset(" << divide_label << ") " << rows << " / " << ROWS << ": " << iteration << " / " << max_iteration << " iteration : " << cluster_changed << " / " << max_cluster_changed << " vectors cluster changed" << std::endl;
			for (auto i = start_label; i < start_label + K; ++i) {
				std::cout << clusters[i].label << '\t' << clusters[i].Count() << std::endl;
			}
		}

		assert(rows == ROWS);
	}

private:
	int K;
	int divide_label;
	int start_label;
	int ROWS;
	DataSet& x;
	Clusters& clusters;
	int iteration;
	int max_iteration;
	int cluster_changed;
	int max_cluster_changed;
	const char* verbose;
};
