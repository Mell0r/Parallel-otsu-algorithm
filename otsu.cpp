#include <iostream>
#include <omp.h>
#include <cmath>

#include "otsu.hpp"

long long safe(long long num) {
	if (num == 0)
		return 1;
	return num;
}

double get_disp_sum(
	const vector<long long>& pref_sum_in_pic,
	const vector<long long>& freq_pref_sum,
	int pic_dim,
	int lev0,
	int lev1,
	int lev2
) {
	return (double)pref_sum_in_pic[lev0] * pref_sum_in_pic[lev0] /
		safe(freq_pref_sum[lev0]) +
		(double)(pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) * (pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) /
		safe(freq_pref_sum[lev1] - freq_pref_sum[lev0]) +
		(double)(pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) * (pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) /
		safe(freq_pref_sum[lev2] - freq_pref_sum[lev1]) +
		(double)(pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) * (pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) /
		safe(freq_pref_sum[pic_dim - 1] - freq_pref_sum[lev2]);
}

void calc_pref_sums(
	vector<long long>& freq,
	vector<long long>& freq_pref_sum,
	vector<long long>& pref_sum_in_pic
) {
	freq_pref_sum = freq;
	freq_pref_sum[0] = freq[0];
	for (int pix = 1; pix < freq.size(); pix++)
		freq_pref_sum[pix] += freq_pref_sum[pix - 1];

	pref_sum_in_pic.resize(freq.size(), 0);
	pref_sum_in_pic[0] = freq[0];
	for (int pix = 1; pix < freq.size(); pix++)
		pref_sum_in_pic[pix] = pref_sum_in_pic[pix - 1] + freq[pix] * (pix + 1);
}

vector<int> calc_max_levels_one_thread(const vector<vector<pixel>>& pic, int pic_dim) {
	vector<long long> freq(pic_dim, 0);
	for (int row = 0; row < pic.size(); row++)
		for (int col = 0; col < pic[0].size(); col++)
			freq[pic[row][col]]++;

	vector<long long> freq_pref_sum, pref_sum_in_pic;
	calc_pref_sums(freq, freq_pref_sum, pref_sum_in_pic);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
	for (int lev0 = 0; lev0 < pic_dim - 2; lev0++) {
		for (int lev1 = lev0 + 1; lev1 < pic_dim - 1; lev1++) {
			for (int lev2 = lev1 + 1; lev2 < pic_dim; lev2++) {
				double disp = get_disp_sum(pref_sum_in_pic, freq_pref_sum, pic_dim, lev0, lev1, lev2);
				if (disp > max_disp) {
					max_disp = disp;
					max_levels = { lev0, lev1, lev2 };
				}
			}
		}
	}
	return max_levels;
}

vector<int> calc_max_levels_multithread(int threads, const vector<vector<pixel>>& pic, int pic_dim) {
	vector<long long> freq(pic_dim, 0);
#pragma omp parallel for num_threads(threads)
	for (int cell = 0; cell < pic.size() * pic[0].size(); cell++) {
		int row = cell / pic[0].size();
		int col = cell % pic[0].size();
#pragma omp atomic
		freq[pic[row][col]]++;
	}

	vector<long long> freq_pref_sum, pref_sum_in_pic;
	calc_pref_sums(freq, freq_pref_sum, pref_sum_in_pic);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
	int mx_sz = pic_dim * pic_dim * pic_dim;

#pragma omp parallel num_threads(threads)
	{
		vector<int> max_levels_local = { 0, 1, 2 };
		double max_disp_local = 0;
#pragma omp for
		for (int x = 0; x < mx_sz; x++) {
			int lev0 = x % pic_dim;
			int lev1 = (x / pic_dim) % pic_dim;
			int lev2 = x / pic_dim / pic_dim;
			if (lev1 <= lev0 || lev2 <= lev1)
				continue;
			double disp = get_disp_sum(pref_sum_in_pic, freq_pref_sum, pic_dim, lev0, lev1, lev2);
			if (disp > max_disp_local) {
				max_disp_local = disp;
				max_levels_local = { lev0, lev1, lev2 };
			}
		}
#pragma omp critical
		{
			if (max_disp_local > max_disp) {
				max_disp = max_disp_local;
				max_levels = max_levels_local;
			}
		}
	}
	return max_levels;
}

vector<int> calc_max_levels_default(int dynamic_blocks, const vector<vector<pixel>>& pic, int pic_dim) {
	vector<long long> freq(pic_dim, 0);
#pragma omp parallel for
	for (int cell = 0; cell < pic.size() * pic[0].size(); cell++) {
		int row = cell / pic[0].size();
		int col = cell % pic[0].size();
#pragma omp atomic
		freq[pic[row][col]]++;
	}

	vector<long long> freq_pref_sum, pref_sum_in_pic;
	calc_pref_sums(freq, freq_pref_sum, pref_sum_in_pic);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
	int mx_sz = pic_dim * pic_dim * pic_dim;

#pragma omp parallel
	{
		vector<int> max_levels_local = { 0, 1, 2 };
		double max_disp_local = 0;
#pragma omp for schedule(dynamic, dynamic_blocks)
		for (int x = 0; x < mx_sz; x++) {
			int lev0 = x % pic_dim;
			int lev1 = (x / pic_dim) % pic_dim;
			int lev2 = x / pic_dim / pic_dim;
			if (lev1 <= lev0 || lev2 <= lev1)
				continue;
			double disp = get_disp_sum(pref_sum_in_pic, freq_pref_sum, pic_dim, lev0, lev1, lev2);
			if (disp > max_disp_local) {
				max_disp_local = disp;
				max_levels_local = { lev0, lev1, lev2 };
			}
		}
#pragma omp critical
		{
			if (max_disp_local > max_disp) {
				max_disp = max_disp_local;
				max_levels = max_levels_local;
			}
		}
	}
	return max_levels;
}

void apply_levels(vector<vector<pixel>>& pic, const vector<int>& levels, const vector<pixel>& maps_to) {
	for (auto& row : pic)
		for (auto& pix : row)
			pix = maps_to[lower_bound(levels.begin(), levels.end(), pix) - levels.begin()];
}