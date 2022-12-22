#include <omp.h>
#include <iostream>

#include "measurements.hpp"
#include "otsu.hpp"

using std::cout;
using std::pair;

void measure_threads_num(vector<vector<pixel>>& pic) {
	vector<pair<int, double>> res;
	for (int thread_num = 1; thread_num <= 32; thread_num <<= 1) {
		volatile double total_time = 0;

		for (int i = 0; i < 30; i++) {
			double start = omp_get_wtime();
			vector<int> max_levels = calc_max_levels_multithread(thread_num, pic, 256);
			apply_levels(pic, max_levels, { 0, 84, 170, 255 });
			double finish = omp_get_wtime();
			total_time += finish - start;
		}
		res.emplace_back(thread_num, total_time / 30 * 1000);
	}
	for (auto e : res)
		cout << e.first << ", ";
	cout << '\n';
	for (auto e : res)
		cout << e.second << ", ";
}

void measure_dynamic_blocks(vector<vector<pixel>>& pic) {
	vector<pair<int, double>> res;
	for (int dynamic_blocks = 1; dynamic_blocks <= 512; dynamic_blocks <<= 1) {
		volatile double total_time = 0;

		for (int i = 0; i < 30; i++) {
			double start = omp_get_wtime();
			vector<int> max_levels = calc_max_levels_default(dynamic_blocks, pic, 256);
			apply_levels(pic, max_levels, { 0, 84, 170, 255 });
			double finish = omp_get_wtime();
			total_time += finish - start;
		}
		res.emplace_back(dynamic_blocks, total_time / 30 * 1000);
	}
	for (auto e : res)
		cout << e.first << ", ";
	cout << '\n';
	for (auto e : res)
		cout << e.second << ", ";
}

void measure_one_thread(vector<vector<pixel>>& pic) {
	volatile double total_time = 0;

	for (int i = 0; i < 30; i++) {
		double start = omp_get_wtime();
		vector<int> max_levels = calc_max_levels_one_thread(pic, 256);
		apply_levels(pic, max_levels, { 0, 84, 170, 255 });
		double finish = omp_get_wtime();
		total_time += finish - start;
	}
	cout << total_time / 30 * 1000;
}