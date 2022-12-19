#include <iostream>
#include <omp.h>

#include "max_levels_calculators.hpp"

long long safe(long long num) {
	if (num == 0)
		return 1;
	return num;
}

vector<int> calc_max_levels_one_thread(const vector<vector<pixel>>& pic, int pic_dim) {
	vector<long long> freq(pic_dim, 0);
	for (int row = 0; row < pic.size(); row++)
		for (int col = 0; col < pic[0].size(); col++)
			freq[pic[row][col]]++;

	vector<long long> freq_pref_sum(freq);
	freq_pref_sum[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		freq_pref_sum[pix] += freq_pref_sum[pix - 1];

	vector<long long> pref_sum_in_pic(pic_dim, 0);
	pref_sum_in_pic[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		pref_sum_in_pic[pix] = pref_sum_in_pic[pix - 1] + freq[pix] * (pix + 1);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
	for (int lev0 = 0; lev0 < pic_dim - 2; lev0++) {
		for (int lev1 = lev0 + 1; lev1 < pic_dim - 1; lev1++) {
			for (int lev2 = lev1 + 1; lev2 < pic_dim; lev2++) {
				double disp = (double)pref_sum_in_pic[lev0] * pref_sum_in_pic[lev0] /
					safe(freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) * (pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) /
					safe(freq_pref_sum[lev1] - freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) * (pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) /
					safe(freq_pref_sum[lev2] - freq_pref_sum[lev1]) +
					(double)(pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) * (pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) /
					safe(freq_pref_sum[pic_dim - 1] - freq_pref_sum[lev2]);
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
#pragma omp parallel for collapse(2)
	for (int row = 0; row < pic.size(); row++)
		for (int col = 0; col < pic[0].size(); col++) {
#pragma omp atomic
			freq[pic[row][col]]++;
		}

	vector<long long> freq_pref_sum(freq);
	freq_pref_sum[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		freq_pref_sum[pix] += freq_pref_sum[pix - 1];

	vector<long long> pref_sum_in_pic(pic_dim, 0);
	pref_sum_in_pic[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		pref_sum_in_pic[pix] = pref_sum_in_pic[pix - 1] + freq[pix] * (pix + 1);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
#pragma omp parallel for schedule(static) collapse(3) num_threads(threads)
	for (int lev0 = 0; lev0 < pic_dim - 2; lev0++) {
		for (int lev1 = 0; lev1 < pic_dim - 1; lev1++) {
			for (int lev2 = 0; lev2 < pic_dim; lev2++) {
				if (lev1 <= lev0 || lev2 <= lev1)
					continue;
				double disp = (double)pref_sum_in_pic[lev0] * pref_sum_in_pic[lev0] /
					safe(freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) * (pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) /
					safe(freq_pref_sum[lev1] - freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) * (pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) /
					safe(freq_pref_sum[lev2] - freq_pref_sum[lev1]) +
					(double)(pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) * (pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) /
					safe(freq_pref_sum[pic_dim - 1] - freq_pref_sum[lev2]);
#pragma omp critical 
				{
					if (disp > max_disp) {
						max_disp = disp;
						max_levels = { lev0, lev1, lev2 };
					}
				}
			}
		}
	}
	return max_levels;
}

vector<int> calc_max_levels_default(const vector<vector<pixel>>& pic, int pic_dim) {
	vector<long long> freq(pic_dim, 0);
	int row, col;
#pragma omp parallel for private(col) collapse(2) 
	for (row = 0; row < pic.size(); row++)
		for (col = 0; col < pic[0].size(); col++)
#pragma omp atomic
			freq[pic[row][col]]++;

	vector<long long> freq_pref_sum(freq);
	freq_pref_sum[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		freq_pref_sum[pix] += freq_pref_sum[pix - 1];

	vector<long long> pref_sum_in_pic(pic_dim, 0);
	pref_sum_in_pic[0] = freq[0];
	for (int pix = 1; pix < pic_dim; pix++)
		pref_sum_in_pic[pix] = pref_sum_in_pic[pix - 1] + freq[pix] * (pix + 1);

	vector<int> max_levels = { 0, 1, 2 };
	double max_disp = 0;
#pragma omp parallel for schedule(static) collapse(3)
	for (int lev0 = 0; lev0 < pic_dim - 2; lev0++) {
		for (int lev1 = 0; lev1 < pic_dim - 1; lev1++) {
			for (int lev2 = 0; lev2 < pic_dim; lev2++) {
				if (lev1 <= lev0 || lev2 <= lev1)
					continue;
				double disp = (double)pref_sum_in_pic[lev0] * pref_sum_in_pic[lev0] /
					safe(freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) * (pref_sum_in_pic[lev1] - pref_sum_in_pic[lev0]) /
					safe(freq_pref_sum[lev1] - freq_pref_sum[lev0]) +
					(double)(pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) * (pref_sum_in_pic[lev2] - pref_sum_in_pic[lev1]) /
					safe(freq_pref_sum[lev2] - freq_pref_sum[lev1]) +
					(double)(pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) * (pref_sum_in_pic[pic_dim - 1] - pref_sum_in_pic[lev2]) /
					safe(freq_pref_sum[pic_dim - 1] - freq_pref_sum[lev2]);
#pragma omp critical 
				{
					if (disp > max_disp) {
						max_disp = disp;
						max_levels = { lev0, lev1, lev2 };
					}
				}
			}
		}
	}
	return max_levels;
}